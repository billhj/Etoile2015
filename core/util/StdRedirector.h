/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file StdRedirector.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include <iostream>

namespace Etoile
{
	template<class Elem = char, class Tr = std::char_traits<Elem >>
	class StdRedirector : public std::basic_streambuf<Elem, Tr>
	{
		
	public:
		/**
		* Callback Function.
		*/
		typedef void (*pfncb) ( const Elem*, std::streamsize _Count, void* pUsrData );

		/**
		* Constructor.
		* @param a_Stream the stream to redirect
		* @param a_Cb the callback function
		* @param a_pUsrData user data passed to callback
		*/
		StdRedirector( std::ostream& a_Stream, pfncb a_Cb, void* a_pUsrData ) :
		  m_Stream( a_Stream ),
			  m_pCbFunc( a_Cb ),
			  m_pUserData( a_pUsrData )
		  {
			  //redirect stream
			  m_pBuf = m_Stream.rdbuf( this );
		  };

		  /**
		  * Destructor.
		  * Restores the original stream.
		  */
		  ~StdRedirector()
		  {
			  m_Stream.rdbuf( m_pBuf );
		  }

		  /**
		  * Override xsputn and make it forward data to the callback function.
		  */
		  std::streamsize xsputn( const Elem* _Ptr, std::streamsize _Count )
		  {
			  m_pCbFunc( _Ptr, _Count, m_pUserData );
			  return _Count;
		  }

		  /**
		  * Override overflow and make it forward data to the callback function.
		  */
		  typename Tr::int_type overflow( typename Tr::int_type v )
		  {
			  Elem ch = Tr::to_char_type( v );
			  //m_pCbFunc( &ch, 1, m_pUserData );
			  return Tr::not_eof( v );
		  }

	protected:
		std::basic_ostream<Elem, Tr>& m_Stream;
		std::streambuf*               m_pBuf;
		pfncb                         m_pCbFunc;
		void*                         m_pUserData;
	};

	template<class Elem = char, class Tr = std::char_traits<Elem >>
	class StdLineRedirector : public StdRedirector<Elem, Tr>
	{
		std::string m_lineBuffer;
	public:

		StdLineRedirector( std::ostream& a_Stream, pfncb a_Cb, void* a_pUsrData ) : StdRedirector(a_Stream, a_Cb, a_pUsrData)
		{
			m_lineBuffer="";
			m_lineBuffer.reserve(4096);
		}

		/**
		* Override xsputn and make it forward data to the callback function.
		*/
		std::streamsize xsputn( const Elem* _Ptr, std::streamsize _Count )
		{
			//m_pCbFunc( _Ptr, _Count, m_pUserData );
			forwardLine( _Ptr, 1);
			return _Count;
		}

		/**
		* Override overflow and make it forward data to the callback function.
		*/
		typename Tr::int_type overflow( typename Tr::int_type v )
		{
			Elem ch = Tr::to_char_type( v );
			//m_pCbFunc( &ch, 1, m_pUserData );
			forwardLine( &ch, 1);
			return Tr::not_eof( v );
		}


	protected:
		void forwardLine(const Elem *_elm, std::streamsize _count) {
			for (std::streamsize i=_count; i-->0; ) {
				if (*_elm=='\n') {
					m_lineBuffer+=*_elm;
					m_pCbFunc( (m_lineBuffer.c_str()), 1, m_pUserData );
					m_lineBuffer="";
					m_lineBuffer.reserve(4096);
				}
				else {
					m_lineBuffer+=*_elm;
				}
				++_elm;
			}
		}

	};


}