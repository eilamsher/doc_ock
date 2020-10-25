#ifndef _ROS_beginner_tutorials_num_h
#define _ROS_beginner_tutorials_num_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace beginner_tutorials
{

  class num : public ros::Msg
  {
    public:
      uint32_t angx_length;
      typedef float _angx_type;
      _angx_type st_angx;
      _angx_type * angx;
      uint32_t angy_length;
      typedef float _angy_type;
      _angy_type st_angy;
      _angy_type * angy;

    num():
      angx_length(0), angx(NULL),
      angy_length(0), angy(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->angx_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->angx_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->angx_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->angx_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->angx_length);
      for( uint32_t i = 0; i < angx_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->angx[i]);
      }
      *(outbuffer + offset + 0) = (this->angy_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->angy_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->angy_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->angy_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->angy_length);
      for( uint32_t i = 0; i < angy_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->angy[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t angx_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      angx_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      angx_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      angx_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->angx_length);
      if(angx_lengthT > angx_length)
        this->angx = (float*)realloc(this->angx, angx_lengthT * sizeof(float));
      angx_length = angx_lengthT;
      for( uint32_t i = 0; i < angx_length; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_angx));
        memcpy( &(this->angx[i]), &(this->st_angx), sizeof(float));
      }
      uint32_t angy_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      angy_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      angy_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      angy_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->angy_length);
      if(angy_lengthT > angy_length)
        this->angy = (float*)realloc(this->angy, angy_lengthT * sizeof(float));
      angy_length = angy_lengthT;
      for( uint32_t i = 0; i < angy_length; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_angy));
        memcpy( &(this->angy[i]), &(this->st_angy), sizeof(float));
      }
     return offset;
    }

    const char * getType(){ return "beginner_tutorials/num"; };
    const char * getMD5(){ return "978776c0adab8bf7ee88ec956d3aefa0"; };

  };

}
#endif
