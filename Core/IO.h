// Copyright 2019 by Sven Fabricius, see LICENSE.txt
/**
 * @file IO.h
 *
 * @brief Basic IO calls
 *
 * @author Sven Fabricius
 * Contact: Sven.Fabricius@livediesel.de
 *
 */

#ifndef CORE_IO_H_
#define CORE_IO_H_

/**
 * Implementation of basic IO calls
 *
 */
class IO_
{
public:
	/**
	 * Resets all outputs to save power
	 */
	static void setupOutputs(void);
};

/**
 * Static class implementation
 */
extern IO_ IO;

#endif /* CORE_IO_H_ */
