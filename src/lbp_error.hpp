#ifndef LBP_ERROR_HPP
#define LBP_ERROR_HPP

#include <stdexcept>

namespace LBP {

	class lbp_error : public std::runtime_error {
		public:
			using std::runtime_error::runtime_error; 
	};

}

#endif