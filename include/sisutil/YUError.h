/* Generated by IDL compiler version OSF DCE T1.2.0-09 */
#ifndef YUError_v0_0_included
#define YUError_v0_0_included

#define ERROR_S_OK (0)
#define ERROR_UNSPECIFIED (0)
#define ERROR_INFORMATIONAL (1)
#define ERROR_WARNING (2)
#define ERROR_ERROR (3)
#define ERROR_FATAL (4)

typedef unsigned long ErrorStatus;
typedef unsigned long SeverityLevel;
typedef unsigned char ErrorText[1025];

typedef struct YUError_struct {
   ErrorStatus   myStatus;
   SeverityLevel mySeverity;
   ErrorText     mytext;
} YUError_struct;
typedef YUError_struct YUError_data;

#endif
