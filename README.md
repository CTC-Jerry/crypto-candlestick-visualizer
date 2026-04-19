# crypto-candlestick-visualizer
C++ terminal-based cryptocurrency exchange simulator — supports order book management, wallet tracking, bid/ask order matching, and ASCII candlestick charting with SOL-USD point plots

## Overview

MerkelMain is an interactive command-line application that simulates a cryptocurrency exchange. Users can analyse market data, place bids and asks, track their wallet, and visualise price history through candlestick charts and point plots — all rendered directly in the terminal.

## Features

- **Order Book** — Load and browse real CSV market data; view ask/bid prices, volumes, and match orders across timesteps
- **Wallet Management** — Track balances across multiple currencies; automatically updated when trades are matched
- **Candlestick Data (Part 1)** — Display paginated OHLC (Open, High, Low, Close) candlestick tables for any product and order type
- **Candlestick Graph (Part 2)** — Render colour-coded ASCII candlestick charts in the terminal (green = bullish, red = bearish, yellow = unchanged)
- **SOL-USD Point Plot (Part 3)** — Visualise Solana price history (open, high, low, close, or volume) as a scrollable point chart with date labels

## Project Structure

```
.
├── main.cpp                  # Entry point
├── MerkelMain.h/.cpp         # Main application loop and menu
├── OrderBook.h/.cpp          # Order book logic and candlestick orchestration
├── OrderBookEntry.h/.cpp     # Order data model
├── CSVReader.h/.cpp          # CSV parsing for both order book and SOL data
├── Candlestick.h/.cpp        # Candlestick data model
├── CandlestickCreator.h/.cpp # OHLC computation from order sets
├── CandlestickPlotting.h/.cpp# ASCII candlestick chart renderer
├── SolCandlestick.h/.cpp     # SOL-USD candlestick data model (extends Candlestick)
├── SolOrder.h/.cpp           # SOL-USD point plot renderer
├── Wallet.h/.cpp             # Wallet logic
├── 20200601.csv              # Main exchange order book data
└── SOL-USD.csv               # Solana historical OHLCV data
```

## Getting Started

### Prerequisites

- A C++17-compatible compiler (e.g. `g++`, `clang++`)
- Terminal with ANSI colour support (for chart colours)

### Build

```bash
g++ -std=c++17 main.cpp MerkelMain.cpp OrderBook.cpp OrderBookEntry.cpp \
    CSVReader.cpp Candlestick.cpp CandlestickCreator.cpp CandlestickPlotting.cpp \
    SolCandlestick.cpp SolOrder.cpp Wallet.cpp -o merkel
```

### Run

```bash
./merkel
```

Make sure `20200601.csv` and `SOL-USD.csv` are in the same directory as the executable.

## Usage

On launch, the application presents a menu:

```
1: Print help
2: Print exchange stats
3: Make an offer
4: Make a bid
5: Print wallet
6: Continue
7: Midterm Part 1 - Show candlestick data
8: Midterm Part 2 - Plot candlestick graph
9: Midterm Part 3 - Plot another data with another graph
```

### Placing Orders

When prompted, enter orders in the format:

```
product,price,amount
```

Example: `ETH/BTC,200,0.5`

### Viewing Candlestick Data (Options 7 & 8)

Enter an order type and product when prompted:

```
ask,ETH/BTC
```

Then navigate with:
- `next` — next page of data
- `prev` — previous page
- `leave` — return to menu

### SOL-USD Plot (Option 9)

Select one of: `open`, `high`, `low`, `close`, `volume`

Navigate with `next`, `prev`, and `leave`.

## How Candlesticks Are Calculated

| Field | Calculation |
|-------|-------------|
| **Open** | Volume-weighted average price of the *previous* timestep |
| **Close** | Volume-weighted average price of the *current* timestep |
| **High** | Maximum price in the current timestep |
| **Low** | Minimum price in the current timestep |

## Data Files

| File | Description |
|------|-------------|
| `20200601.csv` | Exchange order book — columns: `timestamp, product, type, price, amount` |
| `SOL-USD.csv` | SOL/USD daily OHLCV — columns: `date, open, high, low, close, adj_close, volume` |
