#ifndef __M1_TEMPHUMI_H
#define __M1_TEMPHUMI_H

#if COMPILE_TEMPHUMI

void temphumi_open( void );
bool temphumi_active( void );
void temphumi_update( void );

#endif

#endif /* __M1_TEMPHUMI_H */
