---  

# 📈 Deribit Trading System 🚀  

Welcome to the **Deribit Trading System**! This system allows you to interact with Deribit's trading platform, performing various tasks like placing orders, canceling orders, fetching market data, and more. Below is a breakdown of the functionalities and how you can use them. 🎉  

## 🎥 Demo Video  

Want to see the system in action? Watch this [Demonstration Video](https://www.youtube.com/watch?v=Xdlcb2YmZco) showcasing:  
- System functionality  
- Code review  
- Implementation explanation  

---   

## 🔧 Installation Instructions  

### Prerequisites: 
- **Dependencies**: Install `cURL` and ensure it's set up for use in your C++ environment. 

---
### Steps:  
1. **Create a Deribit Account and API Key 🔑**:  
   - Go to the [Deribit Testnet](https://test.deribit.com/) and sign up for an account.  
   - After logging in, navigate to **Account Settings > API Management** to generate an API key.  
   - Copy your **Client ID** and **Client Secret**.
   - Change main.cpp with your Client ID and Client Secret.

2. **Clone the Repository**:  
   ```bash  
   git clone https://github.com/anushka81/Deribit-Trading-System.git  
   cd Deribit-Trading-System  
   ```  

3. **Build the Code**:   
   ```bash  
   g++ -std=c++11 -o trading_system main.cpp -lcurl  
   ```  

4. **Run the Application**:  
   ```bash  
   ./trading_system  
   ```  

5. Follow the interactive menu to place orders, cancel orders, and more!🚀

---  
## Features 🌟  

### 1️⃣ **Place an Order 💲**   
- **User Input**:  
  - **Instrument Name**: The name of the instrument you want to trade.  
  - **Amount**: The amount of the instrument.  
  - **Price**: The price at which you want to place the order.  
- **Response**: You'll get a success message if the order is placed successfully or an error message if something goes wrong.  

---  

### 2️⃣ **Cancel an Order ✖️**    
- **User Input**:  
  - **Order ID**: Enter the ID of the order you want to cancel.  
- **Response**: The status of the cancellation attempt is shown.  

---  

### 3️⃣ **Fetch the Order Book 📖**    
- **User Input**:  
  - **Instrument Name**: The name of the instrument whose order book you want to check.  
- **Response**: Displays the bid and ask prices, along with the quantities available.  

---  

### 4️⃣ **Get Position Details 📊**  
- **User Input**:  
  - **Instrument Name**: The name of the instrument for which you want to view the position.  
- **Response**: Displays detailed position information such as liquidation price, profit/loss, leverage, and more.  

---  

### 5️⃣ **List Supported Instruments 🔢**    
- **Response**: Displays a list of available instruments along with their types and expiry details.  

---  

### 6️⃣ **Modify an Existing Order ✍️**  
- **User Input**:  
  - **Order ID**: The ID of the order you want to modify.  
  - **New Price**: The updated price for the order.  
  - **New Quantity**: The updated quantity for the order.  
- **Response**: Success or error message based on whether the modification was successful.  

--- 

## 🎯 Key Features  
- 🛠️ **Error Handling**: If an API call fails, it retries a configurable number of times before giving up.  
- 🔄 **Retry Logic**: Automatic retries with a delay in case of failures.  
- 💼 **Interactive Menu**: Use a simple menu to perform different trading operations.  

---  

## 🏁 Ready to Start?  

With this tool, you're all set to start trading and managing orders on Deribit! 🎉🚀  

---  

Feel free to fork or modify this repo to suit your trading needs! Happy trading! 🎉📈  
Contact - anniegirdhar08@gmail.com

---  
