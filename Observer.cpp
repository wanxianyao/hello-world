#include <iostream>
#include <list>
using namespace std;

class Observer
{
public:
	virtual void Update() = 0;
};

class Subject
{
public:
	void Attach(Observer* ob)
	{
		m_list.push_back(ob);
	}
	void Detach(Observer* ob)
	{
		list<Observer*>::iterator it = m_list.begin();
		while (it != m_list.end())
		{
			if ((*it) == ob)
			{
				m_list.erase(it);
				break;
			}			
			it++;
		}
	}
	void Notify()
	{
		list<Observer*>::iterator it = m_list.begin();
		while (it != m_list.end())
		{
			(*it)->Update();
			it++;
		}
	}
private:
	list<Observer*> m_list;
};

class MyObserver : public Observer
{
public:
	virtual void Update()
	{
		cout << "update 1" << endl;
	}
};

class MyObserver2 : public Observer
{
public:
	virtual void Update()
	{
		cout << "update 2" << endl;
	}
};

int main(int argc, char *argv[])
{
	MyObserver my1, my3;
	MyObserver2 my2;
	Subject sub;
	sub.Attach(&my1);
	sub.Attach(&my2);
	sub.Attach(&my3);

	sub.Detach(&my3);
	sub.Detach(&my2);

	sub.Notify();
	return 0;
}