#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

#include <iomanip>

class FlexibilityCurve {
private:
    std::vector<int> values;
    std::vector<int> segTree;

    void buildTree(int idx, int left, int right) {
        if (left == right) {
            segTree[idx] = values[left];
            return;
        }
        int mid = (left + right) / 2;
        buildTree(2 * idx, left, mid);
        buildTree(2 * idx + 1, mid + 1, right);
        segTree[idx] = segTree[2 * idx] + segTree[2 * idx + 1];
    }

    int queryTree(int idx, int left, int right, int qLeft, int qRight) const {
        if (qLeft > right || qRight < left) return 0;
        if (qLeft <= left && qRight >= right) return segTree[idx];
        int mid = (left + right) / 2;
        return queryTree(2 * idx, left, mid, qLeft, qRight) +
               queryTree(2 * idx + 1, mid + 1, right, qLeft, qRight);
    }

public:
    FlexibilityCurve(const std::vector<int>& values) : values(values) {
        segTree.resize(4 * values.size(), 0);
        buildTree(1, 0, values.size() - 1);
    }

    const std::vector<int>& getValues() const {
        return values;
    }

    int ramp_up(int interval) const {
        return std::max(values[interval], 0);
    }

    int ramp_down(int interval) const {
        return std::max(-values[interval], 0);
    }

    int total_ramp_up(int start, int end) const {
        int total = queryTree(1, 0, values.size() - 1, start, end);
        return total > 0 ? total : 0;
    }

    int total_ramp_down(int start, int end) const {
        int total = queryTree(1, 0, values.size() - 1, start, end);
        return total < 0 ? -total : 0;
    }

    void print() const {
        std::cout << "FlexibilityCurve(";
        for (size_t i = 0; i < values.size(); ++i) {
            std::cout << values[i];
            if (i != values.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << ")" << std::endl;
    }
};

class Bid {
private:
    int aggregator_id;
    std::vector<std::pair<int, int> > flexibility_resources;
    int valuation;

public:
    Bid(int aggregator_id, const std::vector<std::pair<int, int> >& flexibility_resources, int valuation)
            : aggregator_id(aggregator_id), flexibility_resources(flexibility_resources), valuation(valuation) {}

    int get_aggregator_id() const {
        return aggregator_id;
    }

    const std::vector<std::pair<int, int> >& get_resources() const {
        return flexibility_resources;
    }

    int get_valuation() const {
        return valuation;
    }
};

class DSO {
private:
    FlexibilityCurve flexibility_curve;
    std::vector<Bid> bids;

public:
    DSO(const FlexibilityCurve& curve) : flexibility_curve(curve) {}

    const FlexibilityCurve& getFlexibilityCurve() const {
        return flexibility_curve;
    }

    void receive_bid(const Bid& bid) {
        bids.push_back(bid);
    }

    void evaluate_bids() {
        std::sort(bids.begin(), bids.end(), [](const Bid& a, const Bid& b) {
            return a.get_valuation() > b.get_valuation();
        });

        std::map<int, int> allocated_resources;

        for (const Bid& bid : bids) {
            for (const auto& resource : bid.get_resources()) {
                int interval = resource.first;
                int offered_value = resource.second;
                allocated_resources[interval] += offered_value;
            }
        }

        std::cout << "Allocated resources based on bids:" << std::endl;
        for (const auto& entry : allocated_resources) {
            std::cout << "Interval " << entry.first << ": " << entry.second << " units" << std::endl;
        }
    }
};

class LFM_WDP {
private:
    DSO& dso;
    std::vector<Bid>& bids;

    bool canAcceptBid(const Bid& bid, std::vector<int>& remainingFlexibility) {
        for (const auto& resource : bid.get_resources()) {
            int interval = resource.first;
            int offered_value = resource.second;
            if (remainingFlexibility[interval] - offered_value < 0) {
                return false;
            }
        }
        return true;
    }

public:
    LFM_WDP(DSO& dso, std::vector<Bid>& bids) : dso(dso), bids(bids) {}

    void solve() {
        std::sort(bids.begin(), bids.end(), [](const Bid& a, const Bid& b) {
            return a.get_valuation() > b.get_valuation();
        });

        std::vector<int> remainingFlexibility = dso.getFlexibilityCurve().getValues();
        std::vector<Bid> acceptedBids;

        for (Bid& bid : bids) {
            if (canAcceptBid(bid, remainingFlexibility)) {
                for (const auto& resource : bid.get_resources()) {
                    int interval = resource.first;
                    int offered_value = resource.second;
                    remainingFlexibility[interval] -= offered_value;
                }
                acceptedBids.push_back(bid);
            }
        }

        std::cout << "Accepted bids:" << std::endl;
        for (const Bid& bid : acceptedBids) {
            std::cout << "Bid from aggregator " << bid.get_aggregator_id() << " with valuation " << bid.get_valuation() << std::endl;
        }
    }
};


int main() {
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

    return 0;
}


