/****************************************************************************
 *
 * General Object Type File
 * Copyright (c) 2007 Team Ascent
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0
 * License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a letter to Creative Commons,
 * 543 Howard Street, 5th Floor, San Francisco, California, 94105, USA.
 *
 * EXCEPT TO THE EXTENT REQUIRED BY APPLICABLE LAW, IN NO EVENT WILL LICENSOR BE LIABLE TO YOU
 * ON ANY LEGAL THEORY FOR ANY SPECIAL, INCIDENTAL, CONSEQUENTIAL, PUNITIVE OR EXEMPLARY DAMAGES
 * ARISING OUT OF THIS LICENSE OR THE USE OF THE WORK, EVEN IF LICENSOR HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGES.
 *
 */

#ifndef _SP_GARBAGE_COLLECTOR_H
#define _SP_GARBAGE_COLLECTOR_H

//Reference based smart pointers
//Save while using hahsmaps etc for reference increasing.
//shared pointers will auto delete themselves but even with a purposed delete the pointer will be safe ;P

template <class T> class SharedPointer;
template <class T>
class PointerReference
{
        friend class SharedPointer<T>;
private:
        PointerReference(T* pT) : Count(0), my_pT(pT) { ASSERT(pT != 0); }
        ~PointerReference()         { ASSERT(Count == 0); delete my_pT; }

        unsigned GetRef()  { return ++Count; }
        unsigned FreeRef() {  ASSERT(Count!=0); return --Count; }

        T* const my_pT;
        unsigned Count;
};

template <class T>
class SharedPointer
{
public:
        SharedPointer()
        {
            m_Reference = 0;

        }
        SharedPointer(T* pT)
        {
            if(!pT)  { m_Reference = 0; return; }

            m_Reference = new PointerReference<T>(pT);
            m_Reference->GetRef();

        }

        SharedPointer(const SharedPointer<T>& rVar)
        {
                m_Reference = rVar.m_Reference;
                if (!Null())
                        m_Reference->GetRef();
        }

        ~SharedPointer()
        {
                UnBind();
        }

        T& operator*()
        {
            if (Null())
                    return NULL;
                      //  throw NulRefException();
                return *m_Reference->my_pT;
        }
        T* operator->()
        {
                if (Null())
                    return NULL;
                      //  throw NulRefException();
                return m_Reference->my_pT;
        }
        const T* operator->() const
        {
                if (Null())
                    return NULL;
                        //throw NulRefException();
                return m_Reference->my_pT;
        }

        SharedPointer<T>& operator=(const SharedPointer<T>& rVar)
        {
                if (!rVar.Null())
                        rVar.m_Reference->GetRef();
                UnBind();
                m_Reference = rVar.m_Reference;
                return *this;
        }

        bool operator==(const SharedPointer<T>& lhs)
        {
          
            return this->m_Reference == lhs.m_Reference;
        
        }
        
        bool operator!=(const SharedPointer<T>& lhs)
        {
                return !(*this == lhs);
        }

        bool Null() const {return m_Reference == 0; }
        void SetNull() { UnBind(); }

private:
        void UnBind()
        {
                if (!Null() && m_Reference->FreeRef() == 0)       
                        delete m_Reference;
                m_Reference = 0;
        }
        PointerReference<T>* m_Reference;
};

#endif
