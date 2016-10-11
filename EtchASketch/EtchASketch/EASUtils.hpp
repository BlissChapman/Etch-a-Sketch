//
//  EASUtils.hpp
//  EtchASketch
//
//  Created by Justin Loew on 9/2/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef EASUtils_hpp
#define EASUtils_hpp

#include <stdio.h>
#include <string>

#define EASLog(fmt, ...) etchasketch::_EAS_Log((fmt) , ## __VA_ARGS__)

namespace etchasketch {
	
	double timeFunction(void (function)(void))
	__attribute__((warn_unused_result));
	
	void timeFunctionAndPrint(void (function)(void), std::string funcName);
	
	void _EAS_Log(std::string fmt, ...);
	
}

#endif /* EASUtils_hpp */
