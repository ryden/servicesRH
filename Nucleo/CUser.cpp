/////////////////////////////////////////////////////////////
//
// Servicios de redhispana.org
// Est� prohibida la reproducci�n y el uso parcial o total
// del c�digo fuente de estos servicios sin previa
// autorizaci�n escrita del autor de estos servicios.
//
// Si usted viola esta licencia se emprender�n acciones legales.
//
// (C) RedHispana.Org 2009
//
// Archivo:     CUser.cpp
// Prop�sito:   Usuarios
// Autores:     Alberto Alonso <rydencillo@gmail.com>
//

#include "stdafx.h"

const unsigned long CUser::ms_ulUserModes [ 256 ] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     /* 000-019 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     /* 020-039 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     /* 040-059 */

    0, 0, 0, 0, 0, CUser::UMODE_ADMIN, CUser::UMODE_BOT, 0, CUser::UMODE_DEVELOPER, 0,      /* 060-069 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, CUser::UMODE_LOCOP,                                          /* 070-079 */
    0, 0, CUser::UMODE_ONLYREG, CUser::UMODE_SUSPENDED, 0, 0, 0, 0, CUser::UMODE_VIEWIP, 0, /* 080-089 */
    0, 0, 0, 0, 0, 0, 0, CUser::UMODE_COADMIN, CUser::UMODE_USERBOT, 0,                     /* 090-099 */
    CUser::UMODE_DEAF, 0, 0, CUser::UMODE_DEBUG, CUser::UMODE_HELPOP,                       /* 100-104 */
    CUser::UMODE_INVISIBLE, 0, CUser::UMODE_CHSERV, 0, 0,                                   /* 105-109 */
    CUser::UMODE_IDENTIFIED, CUser::UMODE_OPER, CUser::UMODE_PREOP, 0,                      /* 110-113 */
    CUser::UMODE_REGNICK, CUser::UMODE_SERVNOTICE, 0, 0, 0, CUser::UMODE_WALLOP,            /* 114-119 */
    CUser::UMODE_HIDDENHOST, 0, 0, 0, 0, 0, 0, 0, 0, 0,                                     /* 120-129 */

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     /* 130-149 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     /* 150-169 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     /* 170-189 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     /* 190-209 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     /* 210-229 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,     /* 230-249 */
    0, 0, 0, 0, 0, 0                                                /* 250-255 */
};

CUser::CUser ( )
: m_ulModes ( 0 )
{
    m_bDeletingUser = false;
}

CUser::CUser ( CServer* pServer,
               const CString& szYXX,
               const CString& szName,
               const CString& szIdent,
               const CString& szDesc,
               const CString& szHost,
               unsigned int uiAddress )
: m_ulModes ( 0 )
{
    m_bDeletingUser = false;
    Create ( pServer, szYXX, szName, szIdent, szDesc, szHost, uiAddress );
}

CUser::~CUser ()
{
    m_bDeletingUser = true;

    // Eliminamos al usuario de todos los canales a los que pertenezca
    for ( std::list < CMembership* >::iterator i = m_listMemberships.begin ();
          i != m_listMemberships.end ();
          ++i )
    {
        (*i)->GetChannel ()->RemoveMember ( this );
    }
}

void CUser::Create ( CServer* pServer,
               const CString& szYXX,
               const CString& szName,
               const CString& szIdent,
               const CString& szDesc,
               const CString& szHost,
               unsigned int uiAddress )
{
    CClient::Create ( pServer, szYXX, szName, szDesc );
    m_szIdent = szIdent;
    m_szHost = szHost;
    m_uiAddress = uiAddress;
}


void CUser::FormatNumeric ( char* szDest ) const
{
    const CClient* pParent = GetParent ();
    if ( !pParent || pParent->GetType () != CClient::SERVER )
        *szDest = '\0';
    else
    {
        strcpy ( szDest, pParent->GetYXX () );
        strcat ( szDest, GetYXX () );
    }
}

void CUser::SetNick ( const CString& szNick )
{
    CClient* pParent = GetParent ();
    if ( pParent )
    {
        CServer* pServer = static_cast < CServer* > ( pParent );
        pServer->UpdateUserName ( this, szNick );
    }
    CClient::SetName ( szNick );
}

void CUser::SetModes ( const CString& szModes )
{
    enum { ADD, DEL } eDirection = ADD;

    const char* p = szModes.c_str ();
    while ( *p != '\0' )
    {
        switch ( *p )
        {
            case '+':
            {
                eDirection = ADD;
                break;
            }
            case '-':
            {
                eDirection = DEL;
                break;
            }
            default:
            {
                unsigned long ulFlag = CUser::ms_ulUserModes [ (unsigned char)*p ];
                if ( eDirection == ADD )
                    m_ulModes |= ulFlag;
                else
                    m_ulModes &= ~ulFlag;
            }
        }

        ++p;
    }
}



// Membres�as a canales
void CUser::AddMembership ( CMembership* pMembership )
{
    m_listMemberships.push_back ( pMembership );
}

void CUser::RemoveMembership ( CMembership* pMembership )
{
    if ( !m_bDeletingUser )
        m_listMemberships.remove ( pMembership );
}
