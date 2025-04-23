#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, vector<size_t> shift, size_t nd, size_t nw, size_t row, size_t col);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    const size_t numDay = avail.size(); // avail's row count
    const size_t numWorker = avail[0].size(); // avail's column count

    // initialize 'empty' schedule
    vector<Worker_T> tempRow(dailyNeed, INVALID_ID);
    for(size_t d = 0; d < numDay; d++){
        sched.push_back(tempRow);
    }

    // initialize the shift counts for every worker
    vector<size_t> shift;
    for(size_t w = 0; w < numWorker; w++){
        shift.push_back(0);
    }
    
    // call and return recursive helper
    bool result = scheduleHelper(avail, dailyNeed, maxShifts, sched, shift, numDay, numWorker, 0, 0);
    return result;

}

// recursive helper, row is simply day# and col is simply Worker#
bool scheduleHelper(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched, vector<size_t> shift, size_t nd, size_t nw, size_t row, size_t col){
    // base case: the last day is filled up
    if(row == nd){
        return true;
    }

    // last worker of this day, move to start of next day
    if(col == dailyNeed){
        return scheduleHelper(avail, dailyNeed, maxShifts, sched, shift, nd, nw, row + 1, 0);
    }

    // loop to go through each worker of this day
    for(size_t w = 0; w < nw; w++){
        // if the worker is available today...
        if(avail[row][w] == 1 &&
        // if the worker has fewer shifts than maxShift...
           shift[w] < maxShifts && 
        // if the worker isn't already working
           find(sched[row].begin(), sched[row].end(), w) == sched[row].end())
        {
            sched[row][col] = w;
            shift[w]++;
            
            // recurse to next worker
            if(scheduleHelper(avail, dailyNeed, maxShifts, sched, shift, nd, nw, row, col + 1)){
                return true;
            }

            // backtrack
            shift[w]--;
            sched[row][col] = INVALID_ID;
        } 
    }

    return false;
}