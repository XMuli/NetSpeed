//
// Created by xmuli on 2020/12/23.
//

#ifndef LFXSPEED_LFX_GLOBAL_H
#define LFXSPEED_LFX_GLOBAL_H

#define LFX_NAESPACE lfx

#if !defined(LFX_NAESPACE)
    #define LFX_BEGIN_NAESPACE
    #define LFX_END_NAESPACE
    #define LFX_USE_NAESPACE
#else
    #define LFX_BEGIN_NAESPACE namespace LFX_NAESPACE {
    #define LFX_END_NAESPACE }
    #define LFX_USE_NAESPACE using namespace LFX_NAESPACE;
#endif


#endif //LFXSPEED_LFX_GLOBAL_H
