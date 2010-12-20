/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2010 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _CALLBACK_H
#define _CALLBACK_H

class CallbackBase {
public:
	virtual void execute() = 0;
	virtual ~CallbackBase() {};
};

class CallbackFP {
	typedef void(*callback)();
	callback mycallback;
public:
	CallbackFP(callback cb) : mycallback(cb) {}
	void operator()();
	void execute();
	void set(callback cb) { mycallback = cb; }
	CallbackFP* Create();
};

class CallBackFunctionP0 : public CallbackBase
{
	typedef void(*cbfunction)();
	cbfunction cb;
public:
	CallBackFunctionP0(cbfunction fn) : cb(fn) {}
	void operator()() { return (cb)(); }
	void execute() { return operator()(); }
};

template<typename P1>
class CallBackFunctionP1 : public CallbackBase
{
	typedef void(*cbfunction)(P1);
	cbfunction cb;
	P1 _p1;
public:
	CallBackFunctionP1(cbfunction fn, P1 p1)
	{
		cb = fn;
		_p1 = p1;
	}
	void operator()() { return (cb)(_p1); }
	void execute() { return operator()(); }
};

template<typename P1, typename P2>
class CallBackFunctionP2 : public CallbackBase
{
	typedef void(*cbfunction)(P1,P2);
	cbfunction cb;
	P1 _p1;
	P2 _p2;
public:
	CallBackFunctionP2(cbfunction fn, P1 p1, P2 p2) : cb(fn), _p1(p1), _p2(p2) {}
	void operator()() { return (cb)(_p1,_p2); }
	void execute() { return operator()(); }
};
template<typename P1, typename P2, typename P3>
class CallBackFunctionP3 : public CallbackBase
{
	typedef void(*cbfunction)(P1,P2, P3);
	cbfunction cb;
	P1 _p1;
	P2 _p2;
	P3 _p3;
public:
	CallBackFunctionP3(cbfunction fn, P1 p1, P2 p2, P3 p3) : cb(fn), _p1(p1), _p2(p2), _p3(p3) {};
	void operator()() { return (cb)(_p1,_p2,_p3); }
	void execute() { return operator()(); }
};
template<typename P1, typename P2, typename P3, typename P4>
class CallBackFunctionP4 : public CallbackBase
{
	typedef void(*cbfunction)(P1,P2, P3, P4);
	cbfunction cb;
	P1 _p1;
	P2 _p2;
	P3 _p3;
	P4 _p4;
public:
	CallBackFunctionP4(cbfunction fn, P1 p1, P2 p2, P3 p3, P4 p4) : cb(fn), _p1(p1), _p2(p2), _p3(p3), _p4(p4) {};
	void operator()() { return (cb)(_p1, _p2, _p3, _p4); }
	void execute() { return operator()(); }
};

template < class Class >
class CallbackP0 : public CallbackBase
{
public:

	typedef void (Class::*Method)();
	CallbackP0(Class* _class_instance, Method _method)
	{
	   _obj = _class_instance;
	   _func = _method;
	};
	void operator()() { return (_obj->*_func)(); }
	void execute() { return operator()(); }

private:

	Class*  _obj;
	Method  _func;
};

template < class Class, typename P1 >
class CallbackP1 : public CallbackBase
{
public:

	typedef void (Class::*Method)(P1);
	CallbackP1(Class* _class_instance, Method _method, P1 p1)
	{
	   _obj = _class_instance;
	   _func = _method;
	   _p1 = p1;
	};

	void operator()() { return (_obj->*_func)(_p1); }
	void execute() { return operator()(); }

private:

	Class*  _obj;
	Method  _func;
	P1 _p1;
};

template < class Class, typename P1, typename P2 >
class CallbackP2 : public CallbackBase
{
public:

	typedef void (Class::*Method)(P1, P2);
	CallbackP2(Class* _class_instance, Method _method, P1 p1, P2 p2)
	{
	   _obj = _class_instance;
	   _func = _method;
	   _p1 = p1;
	   _p2 = p2;
	};

	void operator()() { return (_obj->*_func)(_p1, _p2); }
	void execute() { return operator()(); }

private:

	Class*  _obj;
	Method  _func;
	P1 _p1;
	P2 _p2;
};

template < class Class, typename P1, typename P2, typename P3 >
class CallbackP3 : public CallbackBase
{
public:

	typedef void (Class::*Method)(P1, P2, P3);
	CallbackP3(Class* _class_instance, Method _method, P1 p1, P2 p2, P3 p3)
	{
	   _obj = _class_instance;
	   _func = _method;
	   _p1 = p1;
	   _p2 = p2;
	   _p3 = p3;
	};

	void operator()() { return (_obj->*_func)(_p1, _p2, _p3); }
	void execute() { return operator()(); }

private:

	Class*  _obj;
	Method  _func;
	P1 _p1;
	P2 _p2;
	P3 _p3;
};

template < class Class, typename P1, typename P2, typename P3, typename P4 >
class CallbackP4 : public CallbackBase
{
public:

	typedef void (Class::*Method)(P1, P2, P3, P4);
	CallbackP4(Class* _class_instance, Method _method, P1 p1, P2 p2, P3 p3, P4 p4)
	{
	   _obj = _class_instance;
	   _func = _method;
	   _p1 = p1;
	   _p2 = p2;
	   _p3 = p3;
	   _p4 = p4;
	};

	void operator()() { return (_obj->*_func)(_p1, _p2, _p3, _p4); }
	void execute() { return operator()(); }

private:

	Class*  _obj;
	Method  _func;
	P1 _p1;
	P2 _p2;
	P3 _p3;
	P4 _p4;
};

class QueryResult;
struct AsyncQueryResult;
#include <vector>
typedef std::vector<AsyncQueryResult> QueryResultVector;

class SQLCallbackBase
{
public:
	virtual ~SQLCallbackBase();
	virtual void run(QueryResultVector & result) = 0;
};

template<class T>
class SQLClassCallbackP0 : public SQLCallbackBase
{
	typedef void (T::*SCMethod)(QueryResultVector & p);
	T * base;
	SCMethod method;
public:
	SQLClassCallbackP0(T* instance, SCMethod imethod) : SQLCallbackBase(), base(instance), method(imethod) {}
	~SQLClassCallbackP0() {}
	void run(QueryResultVector & data) { (base->*method)(data); }
};

template<class T, typename P1>
class SQLClassCallbackP1 : public SQLCallbackBase
{
	typedef void (T::*SCMethod)(QueryResultVector & p, P1 p1);
	T * base;
	SCMethod method;
	P1 par1;
public:
	SQLClassCallbackP1(T* instance, SCMethod imethod, P1 p1) : SQLCallbackBase(), base(instance), method(imethod), par1(p1) {}
	~SQLClassCallbackP1() {}
	void run(QueryResultVector & data) { (base->*method)(data, par1); }
};

template<class T, typename P1, typename P2>
class SQLClassCallbackP2 : public SQLCallbackBase
{
	typedef void (T::*SCMethod)(QueryResultVector & p, P1 p1, P2 p2);
	T * base;
	SCMethod method;
	P1 par1;
	P2 par2;
public:
	SQLClassCallbackP2(T* instance, SCMethod imethod, P1 p1, P2 p2) : SQLCallbackBase(), base(instance), method(imethod), par1(p1), par2(p2) {}
	~SQLClassCallbackP2() {}
	void run(QueryResultVector & data) { (base->*method)(data, par1, par2); }
};

template<class T, typename P1, typename P2, typename P3>
class SQLClassCallbackP3 : public SQLCallbackBase
{
	typedef void (T::*SCMethod)(QueryResultVector & p, P1 p1, P2 p2, P3 p3);
	T * base;
	SCMethod method;
	P1 par1;
	P2 par2;
	P3 par3;
public:
	SQLClassCallbackP3(T* instance, SCMethod imethod, P1 p1, P2 p2, P3 p3) : SQLCallbackBase(), base(instance), method(imethod), par1(p1), par2(p2), par3(p3) {}
	~SQLClassCallbackP3();
	void run(QueryResultVector & data) { (base->*method)(data, par1, par2, par3); }
};

template<class T, typename P1, typename P2, typename P3, typename P4>
class SQLClassCallbackP4 : public SQLCallbackBase
{
	typedef void (T::*SCMethod)(QueryResultVector & p, P1 p1, P2 p2, P3 p3, P4 p4);
	T * base;
	SCMethod method;
	P1 par1;
	P2 par2;
	P3 par3;
	P4 par4;
public:
	SQLClassCallbackP4(T* instance, SCMethod imethod, P1 p1, P2 p2, P3 p3, P4 p4) : SQLCallbackBase(), base(instance), method(imethod), par1(p1), par2(p2), par3(p3), par4(p4) {}
	~SQLClassCallbackP4() {}
	void run(QueryResultVector & data) { (base->*method)(data, par1, par2, par3, par4); }
};

class SQLFunctionCallbackP0 : public SQLCallbackBase
{
	typedef void(*SCMethod)(QueryResult*);
	SCMethod method;
public:
	SQLFunctionCallbackP0(SCMethod m) : SQLCallbackBase(), method(m) {}
	~SQLFunctionCallbackP0();
	void run(QueryResult * data) { method(data); }
};

template<typename T1>
class SQLFunctionCallbackP1 : public SQLCallbackBase
{
	typedef void(*SCMethod)(QueryResult*, T1 p1);
	SCMethod method;
	T1 p1;
public:
	SQLFunctionCallbackP1(SCMethod m, T1 par1) : SQLCallbackBase(), method(m), p1(par1) {}
	~SQLFunctionCallbackP1();
	void run(QueryResult * data) { method(data, p1); }
};

#endif
