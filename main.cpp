#include <iostream>
#include <string>
#include <curl/curl.h>
#include "include/json.hpp"
#include <chrono>
#include <thread>

using json = nlohmann::json;

// Callback to handle cURL response data
size_t responseHandler(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

// Perform an HTTP POST request with retry logic
std::string makeHttpRequest(const std::string &endpoint, const json &requestBody, const std::string &token = "", int maxRetries = 3, int retryDelay = 2)
{
    std::string response;
    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Failed to initialize cURL." << std::endl;
        return "";
    }

    int retries = 0;
    CURLcode res;
    while (retries < maxRetries)
    {
        curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Convert JSON to string and set it as POST fields
        std::string jsonData = requestBody.dump();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

        // Set headers
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        if (!token.empty())
        {
            headers = curl_slist_append(headers, ("Authorization: Bearer " + token).c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set up response handler
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, responseHandler);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform request
        res = curl_easy_perform(curl);

        if (res == CURLE_OK)
        {
            break; // Success, no need to retry
        }
        else
        {
            std::cerr << "Request error: " << curl_easy_strerror(res) << std::endl;
            if (retries < maxRetries - 1)
            {
                std::cout << "Retrying... (" << retries + 1 << "/" << maxRetries << ")\n";
                std::this_thread::sleep_for(std::chrono::seconds(retryDelay)); // Wait before retrying
            }
            retries++;
        }
    }

    // Clean up
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
        std::cerr << "Failed to complete the request after " << maxRetries << " retries." << std::endl;
        return "";
    }

    return response;
}

// Function to obtain an access token 
std::string fetchAccessToken(const std::string &clientId, const std::string &clientSecret)
{
    json authPayload = {
        {"id", 0},
        {"method", "public/auth"},
        {"params", {{"grant_type", "client_credentials"}, {"client_id", clientId}, {"client_secret", clientSecret}}},
        {"jsonrpc", "2.0"}};

    std::string response = makeHttpRequest("https://test.deribit.com/api/v2/public/auth", authPayload);
    if (response.empty())
    {
        return "";
    }

    json jsonResponse = json::parse(response, nullptr, false);

    if (jsonResponse.contains("result") && jsonResponse["result"].contains("access_token"))
    {
        return jsonResponse["result"]["access_token"];
    }
    else
    {
        std::cerr << "Error: Failed to fetch access token. Response: " << response << std::endl;
        return "";
    }
}

// Function to submit an order
void submitOrder(const std::string &token)
{
    std::string instrument;
    double amount, price;

    std::cout << "Enter instrument name: ";
    std::cin >> instrument;
    std::cout << "Enter amount: ";
    std::cin >> amount;
    std::cout << "Enter price: ";
    std::cin >> price;

    json orderPayload = {
        {"jsonrpc", "2.0"},
        {"method", "private/buy"},
        {"params", {{"instrument_name", instrument}, {"amount", amount}, {"price", price}}},
        {"id", 1}};

    std::string response = makeHttpRequest("https://test.deribit.com/api/v2/private/buy", orderPayload, token);
    if (response.empty())
    {
        std::cerr << "Error: No response received or request failed.\n";
        return;
    }

    json jsonResponse = json::parse(response, nullptr, false);

    if (jsonResponse.contains("result"))
    {
        std::cout << "Order placed successfully: " << jsonResponse["result"] << std::endl;
    }
    else
    {
        std::cerr << "Error placing order. Response: " << response << std::endl;
    }
}

// Function to cancel an existing order 
void cancelExistingOrder(const std::string &token)
{
    std::string orderId;
    std::cout << "Enter order ID to cancel: ";
    std::cin >> orderId;

    json cancelPayload = {
        {"jsonrpc", "2.0"},
        {"method", "private/cancel"},
        {"params", {{"order_id", orderId}}},
        {"id", 2}};

    std::string response = makeHttpRequest("https://test.deribit.com/api/v2/private/cancel", cancelPayload, token);
    if (response.empty())
    {
        std::cerr << "Error: No response received or request failed.\n";
        return;
    }

    std::cout << "Cancel Response: " << response << std::endl;
}

// Function to fetch the order book for a specific instrument 
void getOrderBook()
{
    std::string instrument;
    std::cout << "Enter instrument name: ";
    std::cin >> instrument;
    if (instrument.empty())
    {
        std::cerr << "Error: Instrument name cannot be empty." << std::endl;
        return;
    }

    json orderBookPayload = {
        {"jsonrpc", "2.0"},
        {"method", "public/get_order_book"},
        {"params", {{"instrument_name", instrument}}},
        {"id", 3}};

    int retries = 3; // Maximum number of retries
    std::string response;
    CURLcode res;
    while (retries > 0)
    {
        response = makeHttpRequest("https://test.deribit.com/api/v2/public/get_order_book", orderBookPayload);
        json jsonResponse = json::parse(response, nullptr, false);

        if (jsonResponse.contains("result"))
        {
            const auto &result = jsonResponse["result"];

            std::cout << "\n=== Order Book for " << instrument << " ===\n";
            if (result.contains("bids"))
            {
                std::cout << "Bids:\n";
                for (const auto &bid : result["bids"])
                {
                    std::cout << "Price: " << bid[0] << ", Amount: " << bid[1] << "\n";
                }
            }
            else
            {
                std::cout << "No bids available.\n";
            }

            if (result.contains("asks"))
            {
                std::cout << "\nAsks:\n";
                for (const auto &ask : result["asks"])
                {
                    std::cout << "Price: " << ask[0] << ", Amount: " << ask[1] << "\n";
                }
            }
            else
            {
                std::cout << "No asks available.\n";
            }
            return; // Successfully fetched the order book
        }
        else
        {
            std::cerr << "Error: Unable to fetch order book for " << instrument << ".\n";
            if (retries == 1)
            {
                std::cerr << "Final attempt failed." << std::endl;
                return;
            }
            std::cerr << "Retrying... (" << retries - 1 << " retries left)\n";
            retries--;
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Retry delay
        }
    }
}

// Function to get position details of a specific instrument 
void viewPositions(const std::string &accessToken)
{
    std::string instrument;
    std::cout << "Enter instrument name: ";
    std::cin >> instrument;

    json payload = {
        {"jsonrpc", "2.0"},
        {"method", "private/get_position"},
        {"params", {{"instrument_name", instrument}}},
        {"id", 20}};

    int retries = 3;
    std::string response;
    CURLcode res;
    while (retries > 0)
    {
        response = makeHttpRequest("https://test.deribit.com/api/v2/private/get_position", payload, accessToken);
        json responseJson = json::parse(response, nullptr, false);

        if (responseJson.contains("result"))
        {
            std::cout << "Position Details for " << instrument << ":\n";
            json result = responseJson["result"];
            std::vector<std::string> fields = {
                "estimated_liquidation_price", "size_currency", "realized_funding",
                "total_profit_loss", "realized_profit_loss", "floating_profit_loss",
                "leverage", "average_price", "delta", "interest_value", "mark_price",
                "settlement_price", "index_price", "direction", "open_orders_margin",
                "initial_margin", "maintenance_margin", "kind", "size"};

            for (const auto &field : fields)
            {
                if (result.contains(field))
                {
                    std::cout << field << ": " << result[field] << '\n';
                }
            }
            return; // Successfully fetched position details
        }
        else
        {
            std::cerr << "Error: Could not retrieve position data.\n";
            if (retries == 1)
            {
                std::cerr << "Final attempt failed." << std::endl;
                return;
            }
            std::cerr << "Retrying... (" << retries - 1 << " retries left)\n";
            retries--;
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Retry delay
        }
    }
}

// Function to list all supported instruments
void listInstruments()
{
    std::string endpoint = "https://test.deribit.com/api/v2/public/get_instruments";
    json requestPayload = {
        {"jsonrpc", "2.0"},
        {"method", "public/get_instruments"},
        {"params", {{"currency", "BTC"}, {"kind", "any"}}},
        {"id", 6}};

    std::string response = makeHttpRequest(endpoint, requestPayload);
    json jsonResponse = json::parse(response, nullptr, false);

    if (jsonResponse.contains("result"))
    {
        for (const auto &instrument : jsonResponse["result"])
        {
            std::cout << "Instrument: " << instrument["instrument_name"]
                      << ", Type: " << instrument["kind"]
                      << ", Expiry: " << instrument["expiration_timestamp"] << std::endl;
        }
    }
    else
    {
        std::cerr << "Error fetching instruments." << std::endl;
    }
}

// Function to modify an existing order
void modifyOrder(const std::string &token)
{
    std::string orderId;
    double newPrice, newQuantity;

    std::cout << "Enter Order ID to modify: ";
    std::cin >> orderId;
    std::cout << "Enter new price: ";
    std::cin >> newPrice;
    std::cout << "Enter new quantity: ";
    std::cin >> newQuantity;

    json modifyPayload = {
        {"jsonrpc", "2.0"},
        {"method", "private/edit"},
        {"params", {{"order_id", orderId}, {"amount", newQuantity}, {"price", newPrice}}},
        {"id", 7}};

    int retries = 3;
    std::string response;
    CURLcode res;
    while (retries > 0)
    {
        response = makeHttpRequest("https://test.deribit.com/api/v2/private/edit", modifyPayload, token);
        json jsonResponse = json::parse(response, nullptr, false);

        if (jsonResponse.contains("result"))
        {
            std::cout << "Order modified successfully: " << jsonResponse["result"] << std::endl;
            return; // Successfully modified the order
        }
        else
        {
            std::cerr << "Error modifying order." << std::endl;
            if (retries == 1)
            {
                std::cerr << "Final attempt failed." << std::endl;
                return;
            }
            std::cerr << "Retrying... (" << retries - 1 << " retries left)\n";
            retries--;
            std::this_thread::sleep_for(std::chrono::seconds(2)); // Retry delay
        }
    }
}

// Function to display menu
void tradingMenu(const std::string &token)
{
    int option;
    do
    {
        std::cout << "\n=== 💵💸💵💸💵 Deribit Trading System Menu 💵💸💵💸💵 ===\n";
        std::cout << "1. Place Order 💲\n";
        std::cout << "2. Cancel Order ✖️\n";
        std::cout << "3. Get Order Book 🧾\n";
        std::cout << "4. View Current Positions 💹\n";
        std::cout << "5. List Supported Instruments 💳\n";
        std::cout << "6. Modify Order 💱\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> option;

        switch (option)
        {
        case 1:
            submitOrder(token);
            break;
        case 2:
            cancelExistingOrder(token);
            break;
        case 3:
            getOrderBook();
            break;
        case 4:
            viewPositions(token);
            break;
        case 5:
            listInstruments();
            break;
        case 6:
            modifyOrder(token);
            break;
        case 7:
            std::cout << "Exiting. Thank you!" << std::endl;
            break;
        default:
            std::cerr << "Invalid option. Try again." << std::endl;
        }
    } while (option != 7);
}

int main()
{
    const std::string clientId = "my_client_id";
    const std::string clientSecret = "my_client_secret";

    std::string token = fetchAccessToken(clientId, clientSecret);
    if (!token.empty())
    {
        tradingMenu(token);
    }
    else
    {
        std::cerr << "Authentication failed. Exiting program." << std::endl;
    }

    return 0;
}