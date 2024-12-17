
# 📈 Deribit Trading System 🚀

Welcome to the **Deribit Trading System**! This system allows you to interact with Deribit's trading platform, performing various tasks like placing orders, canceling orders, fetching market data, and more. Below is a breakdown of the functionalities and how you can use them. 🎉

## Features 🌟

### 1️⃣ **Place an Order 💲**
- **Description**: You can place a buy order on the Deribit trading platform.
- **Command**: `submitOrder(token)`
- **User Input**:
  - **Instrument Name**: The name of the instrument you want to trade.
  - **Amount**: The amount of the instrument.
  - **Price**: The price at which you want to place the order.
- **Response**: You'll get a success message if the order is placed successfully or an error message if something goes wrong.

---

### 2️⃣ **Cancel an Order ✖️**
- **Description**: You can cancel an existing order by providing the order ID.
- **Command**: `cancelExistingOrder(token)`
- **User Input**: 
  - **Order ID**: Enter the ID of the order you want to cancel.
- **Response**: The status of the cancellation attempt is shown.

---

### 3️⃣ **Fetch the Order Book 📖**
- **Description**: Retrieve the current order book for any instrument.
- **Command**: `getOrderBook()`
- **User Input**: 
  - **Instrument Name**: The name of the instrument whose order book you want to check.
- **Response**: Displays the bid and ask prices, along with the quantities available.

---

### 4️⃣ **Get Position Details 📊**
- **Description**: View the details of your position for a specific instrument, including profit/loss, leverage, and more.
- **Command**: `viewPositions(token)`
- **User Input**: 
  - **Instrument Name**: The name of the instrument for which you want to view the position.
- **Response**: Displays detailed position information such as liquidation price, profit/loss, leverage, and more.

---

### 5️⃣ **List Supported Instruments 🔢**
- **Description**: Fetch all supported instruments on the platform.
- **Command**: `listInstruments()`
- **Response**: Displays a list of available instruments along with their types and expiry details.

---

### 6️⃣ **Modify an Existing Order ✍️**
- **Description**: Modify an existing order by changing its price or quantity.
- **Command**: `modifyOrder(token)`
- **User Input**: 
  - **Order ID**: The ID of the order you want to modify.
  - **New Price**: The updated price for the order.
  - **New Quantity**: The updated quantity for the order.
- **Response**: Success or error message based on whether the modification was successful.

---

## 🧑‍💻 How It Works:

### **1. Making an HTTP Request 📡**
We use **cURL** for making HTTP POST requests to the Deribit API. The system supports retries in case of failure, ensuring more robust communication with the server. 💪

### **2. Error Handling ⚠️**
The system provides clear error messages, retry logic, and ensures smooth operation even when there are network issues. If a request fails, the system will retry a set number of times before giving up. 🔄

---

## 🎯 Key Features:
- 🛠️ **Error Handling**: If an API call fails, it retries a configurable number of times before giving up.
- 🔄 **Retry Logic**: Automatic retries with a delay in case of failures.
- 💼 **Interactive Menu**: Use a simple menu to perform different trading operations.

---

## 🌍 Let's Start Trading 🚀

To get started, follow the instructions below:

1. **Authenticate**: Fetch your access token by calling the `fetchAccessToken()` function with your **client ID** and **client secret**.
2. **Choose Action**: Use the interactive menu to choose between placing orders, viewing positions, or modifying existing orders.
3. **Execute & Track**: Execute your trading actions and track responses for each action taken.

---

## 📝 Example of Usage:

```cpp
// Step 1: Fetch Access Token
std::string token = fetchAccessToken("your_client_id", "your_client_secret");

// Step 2: Place an Order
submitOrder(token);

// Step 3: View Positions
viewPositions(token);
```

---

## 🏁 Ready to Start? 

With this tool, you're all set to start trading and managing orders on Deribit! 🎉🚀

---

Feel free to fork or modify this repo to suit your trading needs! Happy trading! 🎉📈

