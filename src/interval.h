#ifndef INTERVAL_H
#define INTERVAL_H

/**
 * A helper class used to define and check intervals or ranges of numbers
 */
class interval {
    public:
        
        /**
         * The bounds of the interval
         */
        double min, max;

        /**
         * Creates an empty interval
         */
        interval(): min(+infinity), max(-infinity) {}

        /**
         * Creates an interval from min to max
         * @param min the lower bound of the interval
         * @param max the upper bound of the interval
         */
        interval(double min, double max): min(min), max(max) {}

        /**
         * Creates an interval that tightly bounds around two given intervals
         * @param a interval to surround
         * @param b interval to surround
         */
        interval(const interval& a, const interval& b) {
            min = a.min <= b.min ? a.min : b.min;
            max = a.max >= b.max ? a.max : b.max;
        }

        /**
         * Returns the size of this interval
         * @return size between the min and max of this interval
         */
        double size() const {
            return max-min;
        }

        /**
         * Returns if a given value is contained within the bounds of this interval (inclusive)
         * @param value value to check
         * @return true if value is within the bounds of this interval, false otherwise
         */
        bool contains(double value) const {
            return min <= value && value <= max;
        }

        /**
         * Returns if a given value is contained within the bounds of this interval (exclusive)
         * @param value value to check
         * @return true if value is within the bounds of this interval, false otherwise
         */
        bool surrounds(double value) const {
            return min < value && value < max;
        }

        /**
         * Returns an interval increased in both directions by delta/2
         * @param delta amount to increase by
         * @return a new interval expanded by delta/2 in both directions
         */
        interval expand(double delta) const {
            auto padding = delta/2;
            return interval(min - padding, max + padding);
        }

        /**
         * Constant intervals representing an empty interval and the interval of all real numbers
         */
        static const interval empty, universe;
};

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);
#endif