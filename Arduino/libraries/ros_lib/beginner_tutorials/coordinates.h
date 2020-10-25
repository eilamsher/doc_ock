#ifndef _ROS_beginner_tutorials_coordinates_h
#define _ROS_beginner_tutorials_coordinates_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace beginner_tutorials
{

  class coordinates : public ros::Msg
  {
    public:
      uint32_t datax_length;
      typedef float _datax_type;
      _datax_type st_datax;
      _datax_type * datax;
      uint32_t datay_length;
      typedef float _datay_type;
      _datay_type st_datay;
      _datay_type * datay;
      uint32_t dataz_length;
      typedef float _dataz_type;
      _dataz_type st_dataz;
      _dataz_type * dataz;

    coordinates():
      datax_length(0), datax(NULL),
      datay_length(0), datay(NULL),
      dataz_length(0), dataz(NULL)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->datax_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->datax_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->datax_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->datax_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->datax_length);
      for( uint32_t i = 0; i < datax_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->datax[i]);
      }
      *(outbuffer + offset + 0) = (this->datay_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->datay_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->datay_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->datay_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->datay_length);
      for( uint32_t i = 0; i < datay_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->datay[i]);
      }
      *(outbuffer + offset + 0) = (this->dataz_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->dataz_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->dataz_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->dataz_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->dataz_length);
      for( uint32_t i = 0; i < dataz_length; i++){
      offset += serializeAvrFloat64(outbuffer + offset, this->dataz[i]);
      }
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t datax_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      datax_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      datax_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      datax_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->datax_length);
      if(datax_lengthT > datax_length)
        this->datax = (float*)realloc(this->datax, datax_lengthT * sizeof(float));
      datax_length = datax_lengthT;
      for( uint32_t i = 0; i < datax_length; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_datax));
        memcpy( &(this->datax[i]), &(this->st_datax), sizeof(float));
      }
      uint32_t datay_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      datay_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      datay_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      datay_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->datay_length);
      if(datay_lengthT > datay_length)
        this->datay = (float*)realloc(this->datay, datay_lengthT * sizeof(float));
      datay_length = datay_lengthT;
      for( uint32_t i = 0; i < datay_length; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_datay));
        memcpy( &(this->datay[i]), &(this->st_datay), sizeof(float));
      }
      uint32_t dataz_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      dataz_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      dataz_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      dataz_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->dataz_length);
      if(dataz_lengthT > dataz_length)
        this->dataz = (float*)realloc(this->dataz, dataz_lengthT * sizeof(float));
      dataz_length = dataz_lengthT;
      for( uint32_t i = 0; i < dataz_length; i++){
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->st_dataz));
        memcpy( &(this->dataz[i]), &(this->st_dataz), sizeof(float));
      }
     return offset;
    }

    const char * getType(){ return "beginner_tutorials/coordinates"; };
    const char * getMD5(){ return "6d9fdd56a82d84b20d8a69a500869003"; };

  };

}
#endif
