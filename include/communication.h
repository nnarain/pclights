#ifndef COMMUNICATION_H
#define COMMUNICATION_H

template<class CommT>
class Communcation
{
private:
	CommT comm_;

public:
	template<typename... Args>
	Communcation(Args&... args) :
		comm_(args...)
	{
	}

	void init() 
	{
		comm_.init();
	}
};

#endif 