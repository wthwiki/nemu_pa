#include <isa.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include<malloc.h>
enum {
  /*
  *0-100 op
  * 100-200
  */ 
  OP_TO_NUM=100,
  TK_DIG_B,TK_DIG_O,TK_DIG_D,TK_DIG_X,      //num
  TK_PUS,TK_SUB,TK_MUT,TK_DIV,TK_EQ,TK_NEQ,TK_AND,TK_OR,TK_G,TK_L,TK_LE,TK_GE,               // +-,*,/
  TK_BR_L,TK_BR_R,                          //()
  TK_NOTYPE = 256, 
  TK_REG=301,TK_MEM,TK_POINT,TK_NEG                        // reg mem
  /* TODO: Add more token types */
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {
  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"[ ]+", TK_NOTYPE},    // spaces
  {"[0][x][0-9a-f]{1,16}$",TK_DIG_X},
    {"[1-9][0-9]{0,16}",TK_DIG_D}, //digital
  {"[0][o][0-7]{1,32}",TK_DIG_O},
  {"[+]",TK_PUS},
  {"[-]",TK_SUB},
  {"[*]",TK_MUT},
  {"[/]",TK_DIV},
  {"[(]",TK_BR_L},
  {"[)]",TK_BR_R},
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},
  {"&&", TK_AND},
  {"[|]{2,2}", TK_OR},
  {"<=", TK_LE},
  {">=", TK_GE},
  {">",TK_G},// regex compilation failed: Invalid preceding regular expression >(?!=)
  {"<",TK_L},// regex compilation failed: Invalid preceding regular expression <(?!=)
  {"[$][0-9A-Za-z]{1,5}",TK_REG},  // REG
  {"[$][0][x][0-9a-f]{1,16}",TK_MEM}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;
int eval();
static bool make_token(char *e) {

  printf("e=%s \n",e);
  int position = 0;
  int i;
  regmatch_t pmatch;
  nr_token = 0;
  
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          default: 
            tokens[nr_token].type=rules[i].token_type;
            char ss[32];
	          strcpy(ss,substr_start);
            char* sss=(char*) malloc(sizeof(ss)/sizeof(char));;
	          strcpy(sss,substr_start);
	          char * a=strtok(sss," ");
            strcpy(tokens[nr_token].str,a);
            nr_token++;
            // TODO();
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  return true;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  for (int i = 0; i < nr_token; i ++) {
  if ((tokens[i].type == TK_MUT||tokens[i].type == TK_SUB) && (i == 0 || (tokens[i - 1].type <=TK_GE&&tokens[i - 1].type >=TK_PUS) ) ) {
      if(tokens[i].type == TK_POINT){
        tokens[i].type = TK_POINT;
      }else {
        tokens[i].type = TK_NEG;
      }
    }
  }
  printf("nr_token=%d\n",nr_token);
  word_t res=eval(0,nr_token-1);
  Log("res=%d\n",res);
  return res;
}

bool check_parentheses(int p,int q){
  if(tokens[p].type!=TK_BR_L){
    return false;
  }
  int cnt=0;
  __attribute__((unused)) int type=-1;
  for(int i=p+1;i<=q-1;i++){
    type=tokens[i].type;
    if((type == TK_BR_L)){
      cnt++;
    }else if((type == TK_BR_R)){
      cnt--;
    }
    if(cnt<0){
      return false;
    }
  }
  if(cnt!=0){
    return false;
  }
  return true;
}
bool have_op(int p,int q){
  for(int i=p;i<q;i++){
    if( (tokens[i].type <=TK_GE&&tokens[i].type >=TK_PUS)){
      return true; 
    }
  }
  return false;
}

int eval(int p, int q) {
  if (p > q) {
    printf("p>q gg \n");
    /* Bad expression */
    assert(0);
    
    return 0xffffffff;
  }else if (!have_op(p,q)) {
    printf("no option\n");
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    Token v=tokens[q];
    word_t x;
    const char *name;
    int res;
     __attribute__((unused)) bool f=false;
    switch (v.type){
      case TK_REG:
      name = v.str+1;
      res = isa_reg_str2val(name,&f);
        break;
      case TK_MEM:
        x=atoi(v.str+2);

        res=vaddr_mmu_read(x,4,0);
        break;
      default :
        res=atoi(tokens[q].str);
        break;
    }
    for(int i=q-1;i>=p;i--){
        if(tokens[p].type==TK_NEG){
          printf("neg\n");
          res = -res;
        }else if(tokens[p].type==TK_POINT){
          res = vaddr_mmu_read(res,4,0);
        }
    }
    return res;
  }else if (check_parentheses(p, q) == true) {
    
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1);
  }else {
    int cnt=0;
    int type=-1;
    //op = the position of 主运算符 in the token expression;
    int f=0;
    int op= p ;
    for(int i=p;i<=q;i++){
      type=tokens[i].type;
      if(cnt==0&&(type==TK_DIV||type==TK_MUT)){
        f=1;
        op=i;
      }else if(cnt==0&&f==0&&(type==TK_PUS||type==TK_SUB)){
        printf("iiii\n");
        op=i;
      }else if(type==TK_BR_L){
        cnt++;
      }else if(type==TK_BR_R){
        cnt--;
      }
    }
    int val1 = eval(p, op - 1);
    int val2 = eval(op + 1, q);
    Log("val1=%d,val2=%d,op=%d\n",val1,val2,op);
    switch (tokens[op].type) {
      case TK_PUS: return val1 + val2;
      case TK_SUB: return val1 - val2;
      case TK_MUT: return val1 * val2;
      case TK_DIV: return val1 / val2;
      case TK_EQ: return val1==val2;
      case TK_NEQ:return val1!=val2;
      case TK_AND:return val1&&val2;
      case TK_OR:return val1||val2;
      case TK_LE:return val1<=val2;
      case TK_GE:return val1>=val2;
      case TK_G:return val1>val2;
      case TK_L:return val1<val2;
      default: assert(0);
    }
  }
  return 0xffffffff;
}


// int n1=0;// option num 2;
// int n2=0;// option num 1;
// int n3=0;// tmp num
// int t=-1;// option;
// LinkStack op , num;
// int cal(int num1,int num2,int type){
//   n1=num1;
//   n2=num2;
//   int n=0;
//   switch (type)
//   {
//   case TK_PUS:
//      n=n1+n2;
//      break;
//   case TK_SUB:
//     n=n2-n1;
//     break;
//   case TK_MUT:
//     n=n1*n2;
//     break;
//   case TK_DIV:
//     n= n2/n1;
//     break;
//   default :
//     return -1;
//   }
//   push(&num,n);
// }

// void getNum(){
//     pop(&num,&n1);
//     pop(&num,&n2);
// }
// void show(){
//   printf("n1=%d,n2=%d,n3=%d\n",n1,n2,n3);
// }

// // cal the num when option is mul or div;
// void check_is_mul_of_div(){
//   peek(&op,&t);
//   if(t==TK_MUT||t==TK_DIV){
//     pop(&op,&t);
//     getNum();
//     cal(n1,n2,t);
//     show();
//     pop(&op,&t);
//   }
// }
// // e. expr
// word_t expr(char *e, bool *success) {
//   if (!make_token(e)) {
//     *success = false;
//     return 0;
//   }
//   createStack(&op);
//   createStack(&num);
//   bool f=false;
//   for(int i=0;i<nr_token;i++){
//     int type=tokens[i].type;
//     char* str= tokens[i].str;
//     int val;
//     word_t addr;
//     switch (type){
//       case TK_PUS:
//       case TK_SUB:
//       case TK_MUT:
//       case TK_DIV:
//         push(&op,type);
//         break;
//       case TK_REG:
//             val=getValueByReg(str);
//             push(&num,val);
//             break;
//       case TK_MEM:
//             addr=atoi(str);
//             val=vaddr_mmu_read(addr,4,0);
//             push(&num,val);
//         break;
//       case TK_BR_L:
//         push(&op,type);
//         break;
//       case TK_BR_R:
//         pop(&op,&t);
//         while(t!=TK_BR_L){
//           getNum();
//           n3=cal(n1,n2,t);
//           pop(&op,&t);
//         }
//         check_is_mul_of_div();
//         break;
//       default:
//           val=atoi(str);
//           push(&num,val);
//           check_is_mul_of_div();
//         break;
//     }
//   }
//   State r=pop(&op,&t);
//   while(r==OK){
//     getNum();
//     n3=cal(n1,n2,t);
//     r=pop(&op,&t);
//   }
//   int res=0;
//   pop(&num,&res);
//   printf("res=%d\n",res);
//   return res;
// }
