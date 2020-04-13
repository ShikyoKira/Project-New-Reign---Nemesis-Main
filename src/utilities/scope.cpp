#include "utilities/scope.h"

using namespace std;

nemesis::scope::scope(size_t n_front, size_t n_back, funcptr n_func)
{
	front = n_front;
	back = n_back - 1;
	size = back - front;
	func = n_func;
}

nemesis::scope::scope(size_t n_front, size_t n_back, vecstr n_olddata, funcptr n_func)
{
	front = n_front;
	back = n_back - 1;
	size = back - front;
	olddata = n_olddata;
	func = n_func;
}

nemesis::scope::scope(size_t n_front, size_t n_back, vector<int> n_olddataint, funcptr n_func)
{
	front = n_front;
	back = n_back - 1;
	size = back - front;
	olddataint = n_olddataint;
	func = n_func;
}

nemesis::scope::scope(size_t n_front, size_t n_back, vector<int> n_olddataint, vecstr n_olddata, funcptr n_func)
{
	front = n_front;
	back = n_back - 1;
	size = back - front;
	olddataint = n_olddataint;
	olddata = n_olddata;
	func = n_func;
}
