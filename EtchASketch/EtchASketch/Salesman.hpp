//
//  Salesman.hpp
//  EtchASketch
//
//  Created by Justin Loew on 9/20/16.
//  Copyright © 2016 Justin Loew. All rights reserved.
//

#ifndef Salesman_hpp
#define Salesman_hpp

#include <vector>
#include "KDPoint.hpp"

namespace etchasketch {
namespace salesman {

/// Solves the traveling salesman problem.
class Salesman {

  public:
	Salesman();

	virtual ~Salesman();

	/// Order the points for the best drawing order.
	virtual void orderPoints();

	/// Get a reference to the ordered points.
	const std::vector<etchasketch::KDPoint<2>> & getOrderedPoints() const
	{
		return orderedPoints;
	}

  protected:
	/// The list of points, in the order in which they should be drawn.
	std::vector<etchasketch::KDPoint<2>> orderedPoints;
};

}
}

#endif /* Salesman_hpp */
