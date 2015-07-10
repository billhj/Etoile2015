
#pragma once
#include <windows.h>
class TimeWin32
{
public:
    TimeWin32()
    {
        if ( !ms_bInitialized ) {
            InitFrequency();
        }
        QueryPerformanceCounter( ( LARGE_INTEGER* ) &m_Current );
    }

    float					DiffTime( const TimeWin32* const pTime ) const
    {
        //const TimeWin32* const pTimeWin32 = static_cast< const TimeWin32* >( pTime );
        return ( float ) ( ( ( double )( this->m_Current - pTime->m_Current ) ) / ( double )ms_Frequency ) * 1000.f;
    }

    double getCurrentTime()
    {
        return (double)this->m_Current;
    }


    float					DiffTime( double pTime ) const
    {
        //const TimeWin32* const pTimeWin32 = static_cast< const TimeWin32* >( pTime );
        return ( float ) ( ( ( double )( this->m_Current - pTime) ) / ( double )ms_Frequency ) * 1000.f;
    }

private:
    static void				InitFrequency()
    {
        // frequency
        QueryPerformanceFrequency( ( LARGE_INTEGER* ) &ms_Frequency );
        ms_bInitialized = true;
    }

    __int64					m_Current;

    static __int64			ms_Frequency;
    static bool				ms_bInitialized;
};
