/*****************************************************************************
* \file:   		dtl_dv.c
* \author:		Conny Gustafsson
* \date:		2013-03-07
* \brief:		Dynamic Value
*
* Copyright (c) 2013 Conny Gustafsson
*
******************************************************************************/
#include "dtl_dv.h"
#include "dtl_sv.h"
#include "dtl_av.h"
#include "dtl_hv.h"
#include <malloc.h>
#include "CMemLeak.h"


/**************** Private Function Declarations *******************/
void dtl_dv_create(dtl_dv_t *this);

/**************** Private Variable Declarations *******************/


/****************** Public Function Definitions *******************/
dtl_dv_t *dtl_dv_null(void){
	dtl_dv_t *this;
	if((this = (dtl_dv_t*)malloc(sizeof(dtl_dv_t)))==(dtl_dv_t*)0){
		return (dtl_dv_t*)0;
	}
	dtl_dv_create(this);
	return this;
}

void dtl_dv_delete(dtl_dv_t* dv ){
	if(dv){
		switch(dtl_dv_type(dv))
		{
		case DTL_DV_INVALID:
			break;
		case DTL_DV_NULL:
			free(dv);
			break;
		case DTL_DV_SCALAR:
			dtl_sv_delete((dtl_sv_t*) dv);
			break;
		case DTL_DV_ARRAY:
			dtl_av_delete((dtl_av_t*) dv);
			break;
		case DTL_DV_HASH:
			dtl_hv_delete((dtl_hv_t*) dv);
			break;
		}
	}
}

void dtl_dv_vdelete(void *arg){
   dtl_dv_delete((dtl_dv_t*) arg);
}

void dtl_dv_ref_inc(dtl_dv_t* dv){
	if(dv) dv->u32RefCnt++;
}
void dtl_dv_ref_dec(dtl_dv_t* dv){
	if( (dv) && (dv != (dtl_dv_t*)&g_dtl_sv_undef) && (dv->u32RefCnt>0) )
	{
		if(--dv->u32RefCnt == 0) dtl_dv_delete(dv);
	}
}
dtl_dv_type_id dtl_dv_type(dtl_dv_t* dv){
	uint8_t u8Type;
	if(!dv) return DTL_DV_INVALID;
	u8Type = (uint8_t) ( (dv->u32Flags & DTL_DV_TYPE_MASK) >> DTL_DV_TYPE_SHIFT);
	return (dtl_dv_type_id) u8Type;
}

void dtl_dv_ref_dec_void(void* ptr){
	dtl_dv_ref_dec( (dtl_dv_t*) ptr);
}

/***************** Private Function Definitions *******************/
void dtl_dv_create(dtl_dv_t *this){
	if(this){
		this->pAny = (void*) 0;
		this->u32RefCnt = 1;
		this->u32Flags =  ((uint32_t)DTL_DV_NULL);
	}
}

/************************ Task Definition *************************/

