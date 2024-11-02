#pragma once
#ifndef MACROS_H_
#define MACROS_H_
#define DETAIL_PRINT

typedef  unsigned char BYTE;

#define ERROR 0
#define FROM_FILE 1
#define FROM_USER 2

#define CHECK_RETURN_0(ptr) {\
					 if (ptr==NULL)\
				     return 0;\
					}

#define CHECK_RETURN_NULL(ptr) {\
					 if (ptr==NULL)\
				     return NULL;\
					}

#define CHECK_MSG_RETURN_0(ptr,msg) {\
							if (ptr==NULL){\
							puts(msg);\
							return 0;}\
							}

#define CHECK_0_MSG_CLOSE_FILE(ptr, fileName,msg) {\
								if(ptr==0){\
								fclose(fileName);\
								puts(msg);\
								return 0;}\
								}

#define CHECK_NULL_MSG_CLOSE_FILE(ptr, fileName,msg) {\
								if(ptr==NULL){\
								puts(msg);\
								fclose(fileName);\
								return 0;}\
								}

#define MSG_CLOSE_RETURN_0(fileName,msg) {\
								puts(msg);\
								fclose(fileName);\
								return 0;\
								}
#define MSG_CLOSE_RETURN_NULL(fileName,msg ) {\
								puts(msg);\
								fclose(fileName);\
								return NULL;\
								}

#define ERROR_MSG_PAUSE_RETURN_0(msg)  {\
								 puts(msg);\
								 system("pause");\
								return 0;\
								}
#endif /* MACROS_H_ */
