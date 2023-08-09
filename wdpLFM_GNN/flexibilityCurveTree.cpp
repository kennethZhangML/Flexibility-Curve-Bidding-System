#include <iostream>
#include <vector>
#include <algorithm>

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
