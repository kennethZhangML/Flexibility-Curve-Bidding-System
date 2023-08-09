# Flexibility Curve & Bidding System

This project is an implementation of a flexibility curve, bid, and an evaluation system for distributed system operators (DSOs). It also includes a local flexibility market winner determination problem (LFM-WDP) solver to decide which bids to accept based on their valuation.

## Classes

### 1. FlexibilityCurve

Represents a curve of flexibility intervals and their respective values. It uses a segment tree for efficient querying.

**Methods:**
- `ramp_up(int interval)`: Returns the ramp-up value for the given interval.
- `ramp_down(int interval)`: Returns the ramp-down value for the given interval.
- `total_ramp_up(int start, int end)`: Returns the total ramp-up value for a given range of intervals.
- `total_ramp_down(int start, int end)`: Returns the total ramp-down value for a given range of intervals.
- `print()`: Prints the flexibility curve values.

### 2. Bid

Represents a bid submitted by an aggregator.

**Methods:**
- `get_aggregator_id()`: Returns the aggregator's ID.
- `get_resources()`: Returns the resources offered in the bid.
- `get_valuation()`: Returns the valuation of the bid.

### 3. DSO

Represents a distributed system operator (DSO) that receives and evaluates bids.

**Methods:**
- `receive_bid(const Bid& bid)`: Receives a bid.
- `evaluate_bids()`: Evaluates all received bids and prints allocated resources.

### 4. LFM_WDP

Represents the winner determination problem solver for a local flexibility market.

**Methods:**
- `solve()`: Solves the problem and prints the accepted bids.

## Example Usage

```cpp
std::vector<int> values = {2, -3, 5, -7, 9, -11};

FlexibilityCurve curve(values);
curve.print();

int interval = 2;
int rampUpValue = curve.ramp_up(interval);
int rampDownValue = curve.ramp_down(interval);

std::cout << "Ramp Up Value for interval " << interval << ": " << rampUpValue << std::endl;
std::cout << "Ramp Down Value for interval " << interval << ": " << rampDownValue << std::endl;

int startInterval = 1;
int endInterval = 4;

int totalRampUp = curve.total_ramp_up(startInterval, endInterval);
int totalRampDown = curve.total_ramp_down(startInterval, endInterval);

std::cout << "Total Ramp Up from interval " << startInterval << " to " << endInterval << ": " << totalRampUp << std::endl;
std::cout << "Total Ramp Down from interval " << startInterval << " to " << endInterval << ": " << totalRampDown << std::endl;
