#ifndef ABC
#define ABC


int queue_1(int x, int y)
{
	std::lock_guard<std::mutex> guard(Lock_0);
	if (x = 1)
	{
		Queue_12.push(y);
		return (1);
	}
	else if (x = 2)
	{
		int a = Queue_12.front();
		Queue_12.pop();
		return(a);
	}
	else if (x = 3)
		return(Queue_12.size());
	else
		return 0;
}

#endif 

