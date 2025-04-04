#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

struct MemorySegment {
    int startingPoint;
    int segmentLength;

    MemorySegment(int s, int l) : startingPoint(s), segmentLength(l) {}
};

struct CompareByLength {
    bool operator()(const MemorySegment* a, const MemorySegment* b) const {
        if (a->segmentLength != b->segmentLength)
            return a->segmentLength > b->segmentLength;
        return a->startingPoint < b->startingPoint;
    }
};

struct CompareByStart {
    bool operator()(const MemorySegment* a, const MemorySegment* b) const {
        return a->startingPoint < b->startingPoint;
    }
};

void initializeMemorySegments(std::set<MemorySegment*, CompareByLength>& segmentsByLength, std::set<MemorySegment*, CompareByStart>& segmentsByStart, long long totalSize) {
    MemorySegment* initialSegment = new MemorySegment(1, totalSize);
    segmentsByLength.insert(initialSegment);
    segmentsByStart.insert(initialSegment);
}

void allocateMemorySegment(int reqId, int size, std::set<MemorySegment*, CompareByLength>& segmentsByLength, std::set<MemorySegment*, CompareByStart>& segmentsByStart, std::unordered_map<int, std::pair<int, int>>& allocated, std::vector<int>& output) {
    auto it = segmentsByLength.begin();
    if (it == segmentsByLength.end() || (*it)->segmentLength < size) {
        output.push_back(-1);
        allocated[reqId] = {-1, 0};
    } else {
        MemorySegment* segment = *it;
        int allocatedStart = segment->startingPoint;
        output.push_back(allocatedStart);

        segmentsByLength.erase(it);
        segmentsByStart.erase(segment);

        if (segment->segmentLength > size) {
            MemorySegment* remainingSegment = new MemorySegment(segment->startingPoint + size, segment->segmentLength - size);
            segmentsByLength.insert(remainingSegment);
            segmentsByStart.insert(remainingSegment);
        }

        delete segment;
        allocated[reqId] = {allocatedStart, size};
    }
}

void freeMemorySegment(int reqId, std::set<MemorySegment*, CompareByLength>& segmentsByLength, std::set<MemorySegment*, CompareByStart>& segmentsByStart, std::unordered_map<int, std::pair<int, int>>& allocated) {
    auto itAllocated = allocated.find(reqId);
    if (itAllocated == allocated.end()) return;

    auto [freeStart, freeSize] = itAllocated->second;
    if (freeStart == -1) return;

    allocated.erase(itAllocated);

    MemorySegment* newSegment = new MemorySegment(freeStart, freeSize);
    auto it = segmentsByStart.lower_bound(newSegment);

    if (it != segmentsByStart.begin()) {
        auto itPrev = std::prev(it);
        if ((*itPrev)->startingPoint + (*itPrev)->segmentLength == freeStart) {
            newSegment->startingPoint = (*itPrev)->startingPoint;
            newSegment->segmentLength += (*itPrev)->segmentLength;

            segmentsByLength.erase(*itPrev);
            segmentsByStart.erase(itPrev);
            delete *itPrev;
        }
    }

    if (it != segmentsByStart.end()) {
        if (freeStart + freeSize == (*it)->startingPoint) {
            newSegment->segmentLength += (*it)->segmentLength;

            segmentsByLength.erase(*it);
            segmentsByStart.erase(it);
            delete *it;
        }
    }

    segmentsByLength.insert(newSegment);
    segmentsByStart.insert(newSegment);
}

void displayResults(const std::vector<int>& output) {
    for (int res : output) {
        std::cout << res << "\n";
    }
}

void cleanUpMemory(std::set<MemorySegment*, CompareByLength>& segmentsByLength) {
    for (auto segment : segmentsByLength) {
        delete segment;
    }
}

int main() {
    long long totalMemory;
    int numRequests;
    std::cin >> totalMemory >> numRequests;

    std::set<MemorySegment*, CompareByLength> segmentsByLength;
    std::set<MemorySegment*, CompareByStart> segmentsByStart;
    std::unordered_map<int, std::pair<int, int>> allocatedSegments;
    std::vector<int> outputResults;
    outputResults.reserve(numRequests);

    initializeMemorySegments(segmentsByLength, segmentsByStart, totalMemory);

    for (int i = 1; i <= numRequests; ++i) {
        int request;
        std::cin >> request;

        if (request > 0) {
            allocateMemorySegment(i, request, segmentsByLength, segmentsByStart, allocatedSegments, outputResults);
        } else {
            freeMemorySegment(-request, segmentsByLength, segmentsByStart, allocatedSegments);
        }
    }

    displayResults(outputResults);
    cleanUpMemory(segmentsByLength);

    return 0;
}
