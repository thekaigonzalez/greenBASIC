#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TEMPSIZE        512    /* need bigger sizes for buffer */
#define MAX_STATS           10     /* 10 statements. blunt but like i cant do nothing about it for now */
#define MAX_ARGS            5      /* max 5 arguments -> very blunt but my bedtime is soon and im trying to complete this in 2 hours and ive wasted too much time */

#define S_IDLE              -1     /* instead of obviously using more concentrated enum, we use defines with numbers */
#define S_IGNORANT          0      /* basically strings and anything that ignores all of the other states */
#define S_TNAME             1      /* essentially gathering a type name / starter (PRINT "hello"); PRINT being the type name */
#define S_TPM               2      /* TPM -> T-ype P-ara M-eters*/

#define BA_EOL              '\n'   /* newline to end statement */
#define BA_SPACE            ' '    /* space to separate params and func */
#define BA_EOF              '\0'   /* End Of File */

#define basic_error(x) printf("greenBASIC: error: %s\n", x);exit(-1);

struct GBuffer 
  {
    int state; /* buffers come with states. lets not waste our time here */
    char* buffer;
  };

typedef struct GBuffer BAZBuff;
typedef void (*greenfunc)(char** ARGV);

BAZBuff gbas_new() {
  BAZBuff __newbuffer;

  __newbuffer.state = S_IDLE; // they all start off idle by default
  __newbuffer.buffer = malloc(MAX_TEMPSIZE);

  return __newbuffer;
}

void BASIC_SetState(BAZBuff * __buf, int stat) {
  __buf->state = stat;
}

void PRINT_FUNCTION(char** ARGV) {
  printf("%s\n", *ARGV);
}

void run_greenBASIC(const char* __n)
  {
    BAZBuff _tmp = gbas_new();

    int p; // argument index
    int b; // buffer index
    int l;

    p = 0;
    b = 0;
    l = 0;
    
    char token = *__n;
    char* fname = malloc(MAX_TEMPSIZE);
    
    char* args[MAX_ARGS];

    char* STATS[MAX_STATS][MAX_ARGS+1]; // +1 for FNAME
    
    char*     NAMES[1] = {  "PRINT"          };
    greenfunc FUNCS[1] = {  &PRINT_FUNCTION  };

    BASIC_SetState(&_tmp, S_TNAME);

    while ((*__n++) != BA_EOF) {
      if (token == BA_SPACE && _tmp.state == S_TNAME) {
        BASIC_SetState(&_tmp, S_TPM);

        fname = _tmp.buffer;

        _tmp.buffer = malloc(MAX_TEMPSIZE);
        b = 0;
      } 

      else if ((token == BA_SPACE || token == BA_EOF) && _tmp.state == S_TPM)
      {
      save:
        args[p] = _tmp.buffer;
        _tmp.buffer = malloc(MAX_TEMPSIZE);

        
        b = 0;

        p++;
      }
      
      else if (token == '"' && _tmp.state == S_TPM) {
        BASIC_SetState(&_tmp, S_IGNORANT);
      }

      else if (token == '"' && _tmp.state == S_IGNORANT) {
        BASIC_SetState(&_tmp, S_TPM);
      }

      else if (token == BA_EOL && _tmp.state == S_TPM) {
        bool exists = false;
        for (size_t i = 0; i < sizeof(NAMES) / sizeof(NAMES[0]); i++) {
          if (!strcmp(fname, NAMES[i])) {
            FUNCS[i](args);
            printf("EXISTS\n");
          }
        }

        BASIC_SetState(&_tmp, S_TNAME);
        _tmp.buffer = malloc(MAX_TEMPSIZE);
        
        b = 0;
        p = 0;
      }

      else {
        if (token == BA_SPACE && _tmp.state == S_TNAME)  continue;
          _tmp.buffer[b] = token;
          _tmp.buffer[b+1] = BA_EOF;
          b++;
        }
      token = *__n;
    }
    
    if (strlen(_tmp.buffer) > 0) {
      args[p] = _tmp.buffer;
      _tmp.buffer = malloc(MAX_TEMPSIZE);
        
        b = 0;

        p++;
    }
    if (_tmp.state == S_IGNORANT) basic_error("failed to execute statement due to unfinished quote");
  }

main()
  {
    run_greenBASIC("PRINT \"hello world 1\"\n");

  }
