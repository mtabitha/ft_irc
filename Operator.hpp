#ifndef OPERATOR_HPP
# define OPERATOR_HPP

# include "Client.hpp"

class Operator : public Client {
	private:
		Operator();
	public:
		~Operator();

		void kill();
};

#endif
