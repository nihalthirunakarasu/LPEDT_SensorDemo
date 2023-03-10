/***************************************************************************//**
 * @file
 * @brief Crypto Compatibility Header
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef EM_CRYPTO_COMPAT_H
#define EM_CRYPTO_COMPAT_H

#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_2)

#define CRYPTOACC_IEN_FETCHERENDOFBLOCKIEN                         CRYPTOACC_IEN_FETCHERENDOFBLOCK
#define _CRYPTOACC_IEN_FETCHERENDOFBLOCKIEN_SHIFT                  _CRYPTOACC_IEN_FETCHERENDOFBLOCK_SHIFT
#define _CRYPTOACC_IEN_FETCHERENDOFBLOCKIEN_MASK                   _CRYPTOACC_IEN_FETCHERENDOFBLOCK_MASK
#define _CRYPTOACC_IEN_FETCHERENDOFBLOCKIEN_DEFAULT                _CRYPTOACC_IEN_FETCHERENDOFBLOCK_DEFAULT
#define CRYPTOACC_IEN_FETCHERENDOFBLOCKIEN_DEFAULT                 CRYPTOACC_IEN_FETCHERENDOFBLOCK_DEFAULT

#define CRYPTOACC_IEN_FETCHERSTOPPEDIEN                            CRYPTOACC_IEN_FETCHERSTOPPED
#define _CRYPTOACC_IEN_FETCHERSTOPPEDIEN_SHIFT                     _CRYPTOACC_IEN_FETCHERSTOPPED_SHIFT
#define _CRYPTOACC_IEN_FETCHERSTOPPEDIEN_MASK                      _CRYPTOACC_IEN_FETCHERSTOPPED_MASK
#define _CRYPTOACC_IEN_FETCHERSTOPPEDIEN_DEFAULT                   _CRYPTOACC_IEN_FETCHERSTOPPED_DEFAULT
#define CRYPTOACC_IEN_FETCHERSTOPPEDIEN_DEFAULT                    CRYPTOACC_IEN_FETCHERSTOPPED_DEFAULT

#define CRYPTOACC_IEN_FETCHERERRORIEN                              CRYPTOACC_IEN_FETCHERERROR
#define _CRYPTOACC_IEN_FETCHERERRORIEN_SHIFT                       _CRYPTOACC_IEN_FETCHERERROR_SHIFT
#define _CRYPTOACC_IEN_FETCHERERRORIEN_MASK                        _CRYPTOACC_IEN_FETCHERERROR_MASK
#define _CRYPTOACC_IEN_FETCHERERRORIEN_DEFAULT                     _CRYPTOACC_IEN_FETCHERERROR_DEFAULT
#define CRYPTOACC_IEN_FETCHERERRORIEN_DEFAULT                      CRYPTOACC_IEN_FETCHERERROR_DEFAULT

#define CRYPTOACC_IEN_PUSHERENDOFBLOCKIEN                          CRYPTOACC_IEN_PUSHERENDOFBLOCK
#define _CRYPTOACC_IEN_PUSHERENDOFBLOCKIEN_SHIFT                   _CRYPTOACC_IEN_PUSHERENDOFBLOCK_SHIFT
#define _CRYPTOACC_IEN_PUSHERENDOFBLOCKIEN_MASK                    _CRYPTOACC_IEN_PUSHERENDOFBLOCK_MASK
#define _CRYPTOACC_IEN_PUSHERENDOFBLOCKIEN_DEFAULT                 _CRYPTOACC_IEN_PUSHERENDOFBLOCK_DEFAULT
#define CRYPTOACC_IEN_PUSHERENDOFBLOCKIEN_DEFAULT                  CRYPTOACC_IEN_PUSHERENDOFBLOCK_DEFAULT

#define CRYPTOACC_IEN_PUSHERSTOPPEDIEN                             CRYPTOACC_IEN_PUSHERSTOPPED
#define _CRYPTOACC_IEN_PUSHERSTOPPEDIEN_SHIFT                      _CRYPTOACC_IEN_PUSHERSTOPPED_SHIFT
#define _CRYPTOACC_IEN_PUSHERSTOPPEDIEN_MASK                       _CRYPTOACC_IEN_PUSHERSTOPPED_MASK
#define _CRYPTOACC_IEN_PUSHERSTOPPEDIEN_DEFAULT                    _CRYPTOACC_IEN_PUSHERSTOPPED_DEFAULT
#define CRYPTOACC_IEN_PUSHERSTOPPEDIEN_DEFAULT                     CRYPTOACC_IEN_PUSHERSTOPPED_DEFAULT

#define CRYPTOACC_IEN_PUSHERERRORIEN                               CRYPTOACC_IEN_PUSHERERROR
#define _CRYPTOACC_IEN_PUSHERERRORIEN_SHIFT                        _CRYPTOACC_IEN_PUSHERERROR_SHIFT
#define _CRYPTOACC_IEN_PUSHERERRORIEN_MASK                         _CRYPTOACC_IEN_PUSHERERROR_MASK
#define _CRYPTOACC_IEN_PUSHERERRORIEN_DEFAULT                      _CRYPTOACC_IEN_PUSHERERROR_DEFAULT
#define CRYPTOACC_IEN_PUSHERERRORIEN_DEFAULT                       CRYPTOACC_IEN_PUSHERERROR_DEFAULT

#define CRYPTOACC_IF_FETCHERENDOFBLOCKIF                          CRYPTOACC_IF_FETCHERENDOFBLOCK
#define _CRYPTOACC_IF_FETCHERENDOFBLOCKIF_SHIFT                   _CRYPTOACC_IF_FETCHERENDOFBLOCK_SHIFT
#define _CRYPTOACC_IF_FETCHERENDOFBLOCKIF_MASK                    _CRYPTOACC_IF_FETCHERENDOFBLOCK_MASK
#define _CRYPTOACC_IF_FETCHERENDOFBLOCKIF_DEFAULT                 _CRYPTOACC_IF_FETCHERENDOFBLOCK_DEFAULT
#define CRYPTOACC_IF_FETCHERENDOFBLOCKIF_DEFAULT                  CRYPTOACC_IF_FETCHERENDOFBLOCK_DEFAULT

#define CRYPTOACC_IF_FETCHERSTOPPEDIF                             CRYPTOACC_IF_FETCHERSTOPPED
#define _CRYPTOACC_IF_FETCHERSTOPPEDIF_SHIFT                      _CRYPTOACC_IF_FETCHERSTOPPED_SHIFT
#define _CRYPTOACC_IF_FETCHERSTOPPEDIF_MASK                       _CRYPTOACC_IF_FETCHERSTOPPED_MASK
#define _CRYPTOACC_IF_FETCHERSTOPPEDIF_DEFAULT                    _CRYPTOACC_IF_FETCHERSTOPPED_DEFAULT
#define CRYPTOACC_IF_FETCHERSTOPPEDIF_DEFAULT                     CRYPTOACC_IF_FETCHERSTOPPED_DEFAULT

#define CRYPTOACC_IF_FETCHERERRORIF                               CRYPTOACC_IF_FETCHERERROR
#define _CRYPTOACC_IF_FETCHERERRORIF_SHIFT                        _CRYPTOACC_IF_FETCHERERROR_SHIFT
#define _CRYPTOACC_IF_FETCHERERRORIF_MASK                         _CRYPTOACC_IF_FETCHERERROR_MASK
#define _CRYPTOACC_IF_FETCHERERRORIF_DEFAULT                      _CRYPTOACC_IF_FETCHERERROR_DEFAULT
#define CRYPTOACC_IF_FETCHERERRORIF_DEFAULT                       CRYPTOACC_IF_FETCHERERROR_DEFAULT

#define CRYPTOACC_IF_PUSHERENDOFBLOCKIF                           CRYPTOACC_IF_PUSHERENDOFBLOCK
#define _CRYPTOACC_IF_PUSHERENDOFBLOCKIF_SHIFT                    _CRYPTOACC_IF_PUSHERENDOFBLOCK_SHIFT
#define _CRYPTOACC_IF_PUSHERENDOFBLOCKIF_MASK                     _CRYPTOACC_IF_PUSHERENDOFBLOCK_MASK
#define _CRYPTOACC_IF_PUSHERENDOFBLOCKIF_DEFAULT                  _CRYPTOACC_IF_PUSHERENDOFBLOCK_DEFAULT
#define CRYPTOACC_IF_PUSHERENDOFBLOCKIF_DEFAULT                   CRYPTOACC_IF_PUSHERENDOFBLOCK_DEFAULT

#define CRYPTOACC_IF_PUSHERSTOPPEDIF                              CRYPTOACC_IF_PUSHERSTOPPED
#define _CRYPTOACC_IF_PUSHERSTOPPEDIF_SHIFT                       _CRYPTOACC_IF_PUSHERSTOPPED_SHIFT
#define _CRYPTOACC_IF_PUSHERSTOPPEDIF_MASK                        _CRYPTOACC_IF_PUSHERSTOPPED_MASK
#define _CRYPTOACC_IF_PUSHERSTOPPEDIF_DEFAULT                     _CRYPTOACC_IF_PUSHERSTOPPED_DEFAULT
#define CRYPTOACC_IF_PUSHERSTOPPEDIF_DEFAULT                      CRYPTOACC_IF_PUSHERSTOPPED_DEFAULT

#define CRYPTOACC_IF_PUSHERERRORIF                                CRYPTOACC_IF_PUSHERERROR
#define _CRYPTOACC_IF_PUSHERERRORIF_SHIFT                         _CRYPTOACC_IF_PUSHERERROR_SHIFT
#define _CRYPTOACC_IF_PUSHERERRORIF_MASK                          _CRYPTOACC_IF_PUSHERERROR_MASK
#define _CRYPTOACC_IF_PUSHERERRORIF_DEFAULT                       _CRYPTOACC_IF_PUSHERERROR_DEFAULT
#define CRYPTOACC_IF_PUSHERERRORIF_DEFAULT                        CRYPTOACC_IF_PUSHERERROR_DEFAULT

#define CRYPTOACC_IF_CLR_FETCHERENDOFBLOCKIFC                     CRYPTOACC_IF_CLR_FETCHERENDOFBLOCK
#define _CRYPTOACC_IF_CLR_FETCHERENDOFBLOCKIFC_SHIFT              _CRYPTOACC_IF_CLR_FETCHERENDOFBLOCK_SHIFT
#define _CRYPTOACC_IF_CLR_FETCHERENDOFBLOCKIFC_MASK               _CRYPTOACC_IF_CLR_FETCHERENDOFBLOCK_MASK
#define _CRYPTOACC_IF_CLR_FETCHERENDOFBLOCKIFC_DEFAULT            _CRYPTOACC_IF_CLR_FETCHERENDOFBLOCK_DEFAULT
#define CRYPTOACC_IF_CLR_FETCHERENDOFBLOCKIFC_DEFAULT             CRYPTOACC_IF_CLR_FETCHERENDOFBLOCK_DEFAULT

#define CRYPTOACC_IF_CLR_FETCHERSTOPPEDIFC                        CRYPTOACC_IF_CLR_FETCHERSTOPPED
#define _CRYPTOACC_IF_CLR_FETCHERSTOPPEDIFC_SHIFT                 _CRYPTOACC_IF_CLR_FETCHERSTOPPED_SHIFT
#define _CRYPTOACC_IF_CLR_FETCHERSTOPPEDIFC_MASK                  _CRYPTOACC_IF_CLR_FETCHERSTOPPED_MASK
#define _CRYPTOACC_IF_CLR_FETCHERSTOPPEDIFC_DEFAULT               _CRYPTOACC_IF_CLR_FETCHERSTOPPED_DEFAULT
#define CRYPTOACC_IF_CLR_FETCHERSTOPPEDIFC_DEFAULT                CRYPTOACC_IF_CLR_FETCHERSTOPPED_DEFAULT

#define CRYPTOACC_IF_CLR_FETCHERERRORIFC                          CRYPTOACC_IF_CLR_FETCHERERROR
#define _CRYPTOACC_IF_CLR_FETCHERERRORIFC_SHIFT                   _CRYPTOACC_IF_CLR_FETCHERERROR_SHIFT
#define _CRYPTOACC_IF_CLR_FETCHERERRORIFC_MASK                    _CRYPTOACC_IF_CLR_FETCHERERROR_MASK
#define _CRYPTOACC_IF_CLR_FETCHERERRORIFC_DEFAULT                 _CRYPTOACC_IF_CLR_FETCHERERROR_DEFAULT
#define CRYPTOACC_IF_CLR_FETCHERERRORIFC_DEFAULT                  CRYPTOACC_IF_CLR_FETCHERERROR_DEFAULT

#define CRYPTOACC_IF_CLR_PUSHERENDOFBLOCKIFC                      CRYPTOACC_IF_CLR_PUSHERENDOFBLOCK
#define _CRYPTOACC_IF_CLR_PUSHERENDOFBLOCKIFC_SHIFT               _CRYPTOACC_IF_CLR_PUSHERENDOFBLOCK_SHIFT
#define _CRYPTOACC_IF_CLR_PUSHERENDOFBLOCKIFC_MASK                _CRYPTOACC_IF_CLR_PUSHERENDOFBLOCK_MASK
#define _CRYPTOACC_IF_CLR_PUSHERENDOFBLOCKIFC_DEFAULT             _CRYPTOACC_IF_CLR_PUSHERENDOFBLOCK_DEFAULT
#define CRYPTOACC_IF_CLR_PUSHERENDOFBLOCKIFC_DEFAULT              CRYPTOACC_IF_CLR_PUSHERENDOFBLOCK_DEFAULT

#define CRYPTOACC_IF_CLR_PUSHERSTOPPEDIFC                         CRYPTOACC_IF_CLR_PUSHERSTOPPED
#define _CRYPTOACC_IF_CLR_PUSHERSTOPPEDIFC_SHIFT                  _CRYPTOACC_IF_CLR_PUSHERSTOPPED_SHIFT
#define _CRYPTOACC_IF_CLR_PUSHERSTOPPEDIFC_MASK                   _CRYPTOACC_IF_CLR_PUSHERSTOPPED_MASK
#define _CRYPTOACC_IF_CLR_PUSHERSTOPPEDIFC_DEFAULT                _CRYPTOACC_IF_CLR_PUSHERSTOPPED_DEFAULT
#define CRYPTOACC_IF_CLR_PUSHERSTOPPEDIFC_DEFAULT                 CRYPTOACC_IF_CLR_PUSHERSTOPPED_DEFAULT

#define CRYPTOACC_IF_CLR_PUSHERERRORIFC                           CRYPTOACC_IF_CLR_PUSHERERROR
#define _CRYPTOACC_IF_CLR_PUSHERERRORIFC_SHIFT                    _CRYPTOACC_IF_CLR_PUSHERERROR_SHIFT
#define _CRYPTOACC_IF_CLR_PUSHERERRORIFC_MASK                     _CRYPTOACC_IF_CLR_PUSHERERROR_MASK
#define _CRYPTOACC_IF_CLR_PUSHERERRORIFC_DEFAULT                  _CRYPTOACC_IF_CLR_PUSHERERROR_DEFAULT
#define CRYPTOACC_IF_CLR_PUSHERERRORIFC_DEFAULT                   CRYPTOACC_IF_CLR_PUSHERERROR_DEFAULT

#endif /* _SILICON_LABS_32B_SERIES_2_CONFIG_2 */
#endif /* EM_CRYPTO_COMPAT_H */
