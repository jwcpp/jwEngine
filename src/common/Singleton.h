
#ifndef SRC_UTILS_SINGLETON_H_
#define SRC_UTILS_SINGLETON_H_

template <class T>
class Singleton
{
public:
    static T* Instance() {
        static T _instance;
        return &_instance;
    }

protected:
    // Disable compiler generated code to prevent default constuction/destruction
    // and copy/assignment semantics.
	Singleton(){}
	~Singleton(){}
	Singleton(Singleton const&){}
	Singleton& operator=(Singleton const&){}
};


#endif  // SRC_UTILS_SINGLETON_H_
