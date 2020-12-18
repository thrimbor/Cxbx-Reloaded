// ******************************************************************
// *
// *  This file is part of the Cxbx project.
// *
// *  Cxbx and Cxbe are free software; you can redistribute them
// *  and/or modify them under the terms of the GNU General Public
// *  License as published by the Free Software Foundation; either
// *  version 2 of the license, or (at your option) any later version.
// *
// *  This program is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// *  GNU General Public License for more details.
// *
// *  You should have recieved a copy of the GNU General Public License
// *  along with this program; see the file COPYING.
// *  If not, write to the Free Software Foundation, Inc.,
// *  59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// *
// *  (c) 2020 Stefan Schmidt
// *
// *  All rights reserved
// *
// ******************************************************************

#define LOG_PREFIX CXBXR_MODULE::XONLINE

#include "common\Logging.h"
#include "common\util\strConverter.hpp"
#include "core\hle\XONLINE\XOnline.h"
#include "core\hle\Intercept.hpp"

#define XB_TRAMPOLINES(XB_MACRO)                                                                                                                                                                                  \
    XB_MACRO(xbox::hresult_xt, WINAPI, XOnlineMatchSearch, (xbox::dword_xt, xbox::dword_xt, xbox::dword_xt, const xbox::XONLINE_ATTRIBUTE*, xbox::dword_xt, HANDLE, xbox::PXONLINETASK_HANDLE));                  \
    XB_MACRO(xbox::hresult_xt, WINAPI, XOnlineMatchSearchResultsLen, (xbox::dword_xt, xbox::dword_xt, const xbox::XONLINE_ATTRIBUTE_SPEC*));                                                                            \
    XB_MACRO(xbox::hresult_xt, WINAPI, XOnlineMatchSessionCreate, (xbox::dword_xt, xbox::dword_xt, xbox::dword_xt, xbox::dword_xt, xbox::dword_xt, xbox::PXONLINE_ATTRIBUTE, HANDLE, xbox::PXONLINETASK_HANDLE));

XB_TRAMPOLINES(XB_trampoline_declare);

void LookupXonlineTrampolines()
{
    XB_TRAMPOLINES(XB_trampoline_lookup);
}

#undef XB_TRAMPOLINES

// ******************************************************************
// * patch: XOnlineMatchSearch
// ******************************************************************
xbox::hresult_xt WINAPI xbox::EMUPATCH(XOnlineMatchSearch)
(
    dword_xt dwProcedureIndex,
    dword_xt dwNumResults,
    dword_xt dwNumAttributes,
    const XONLINE_ATTRIBUTE* pAttributes,
    dword_xt dwResultsLen,
    HANDLE hWorkEvent,
    PXONLINETASK_HANDLE phTask
    )
{
    LOG_FUNC_BEGIN
        LOG_FUNC_ARG(dwProcedureIndex)
        LOG_FUNC_ARG(dwNumResults)
        LOG_FUNC_ARG(dwNumAttributes)
        LOG_FUNC_ARG(pAttributes)
        LOG_FUNC_ARG(dwResultsLen)
        LOG_FUNC_ARG(hWorkEvent)
        LOG_FUNC_ARG(phTask)
        LOG_FUNC_END;

    std::stringstream s;

    s << "XOnlineMatchSearch attributes: " << std::endl << std::hex;

    for (dword_xt i = 0; i < dwNumAttributes; i++) {
        s << "[" << i << "] ID: 0x" << pAttributes[i].dwAttributeID << " ";
        if (pAttributes[i].dwAttributeID & 0xF00000)
            s << "Type: null" << std::endl;
        else if (pAttributes[i].dwAttributeID & 0x100000)
            s << "Type: string Content: " << utf16_to_ascii(reinterpret_cast<wchar_t*>(pAttributes[i].string.lpValue)) << std::endl;
        else
            s << "Type: int Content: " << pAttributes[i].integer.qwValue << std::endl;
    }

    EmuLog(LOG_LEVEL::DEBUG, s.str().c_str());

    hresult_xt result = XB_TRMP(XOnlineMatchSearch)(dwProcedureIndex, dwNumResults, dwNumAttributes, pAttributes, dwResultsLen, hWorkEvent, phTask);

    RETURN(result);
}

// ******************************************************************
// * patch: XOnlineMatchSearchResultsLen
// ******************************************************************
xbox::hresult_xt WINAPI xbox::EMUPATCH(XOnlineMatchSearchResultsLen)
(
    xbox::dword_xt dwNumResults,
    xbox::dword_xt dwNumSessionAttributes,
    const xbox::XONLINE_ATTRIBUTE_SPEC* pSessionAttributeSpec
    )
{
    LOG_FUNC_BEGIN
        LOG_FUNC_ARG(dwNumResults)
        LOG_FUNC_ARG(dwNumSessionAttributes)
        LOG_FUNC_ARG(pSessionAttributeSpec)
    LOG_FUNC_END;

    std::stringstream s;

    s << "XOnlineMatchSearchResultsLen attributes: " << std::endl << std::hex;

    for (dword_xt i = 0; i < dwNumSessionAttributes; i++) {
        s << "[" << i << "] ID: 0x" << pSessionAttributeSpec[i].dwType << " ";
        if (pSessionAttributeSpec[i].dwType & 0x100000)
            s << "Type: string" << std::endl;
        else
            s << "Type: int" << std::endl;
    }

    EmuLog(LOG_LEVEL::DEBUG, s.str().c_str());

    hresult_xt result = XB_TRMP(XOnlineMatchSearchResultsLen)(dwNumResults, dwNumSessionAttributes, pSessionAttributeSpec);

    RETURN(result);
}

// ******************************************************************
// * patch: XOnlineMatchSessionCreate
// ******************************************************************
xbox::hresult_xt WINAPI xbox::EMUPATCH(XOnlineMatchSessionCreate)
(
    dword_xt dwPublicCurrent,
    dword_xt dwPublicAvailable,
    dword_xt dwPrivateCurrent,
    dword_xt dwPrivateAvailable,
    dword_xt dwNumAttributes,
    PXONLINE_ATTRIBUTE pAttributes,
    HANDLE hWorkEvent,
    PXONLINETASK_HANDLE phTask
    )
{
    
	LOG_FUNC_BEGIN
		LOG_FUNC_ARG(dwPublicCurrent)
		LOG_FUNC_ARG(dwPublicAvailable)
        LOG_FUNC_ARG(dwPrivateCurrent)
        LOG_FUNC_ARG(dwPrivateAvailable)
        LOG_FUNC_ARG(dwNumAttributes)
        LOG_FUNC_ARG(pAttributes)
        LOG_FUNC_ARG(hWorkEvent)
        LOG_FUNC_ARG(phTask)
	LOG_FUNC_END;

    std::stringstream s;

    s << "XOnlineMatchSessionCreate attributes: " << std::endl << std::hex;

    for (dword_xt i = 0; i < dwNumAttributes; i++) {
        s << "[" << i << "] ID: 0x" << pAttributes[i].dwAttributeID << " ";
        if (pAttributes[i].dwAttributeID & 0x100000)
            s << "Type: string Content: " << utf16_to_ascii(reinterpret_cast<wchar_t*>(pAttributes[i].string.lpValue)) << std::endl;
        else
            s << "Type: int Content: " << pAttributes[i].integer.qwValue << std::endl;
    }

    EmuLog(LOG_LEVEL::DEBUG, s.str().c_str());

    hresult_xt result = XB_TRMP(XOnlineMatchSessionCreate)(dwPublicCurrent, dwPublicAvailable, dwPrivateCurrent, dwPrivateAvailable, dwNumAttributes, pAttributes, hWorkEvent, phTask);

	RETURN(result);
}
