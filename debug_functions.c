#include "chibicc.h"

void print_headers(void) {
  printf("id_literal,tok_kind,val,float_val,type_kind,type_size,type_alignment"
      ",no_nodes,array_len,line_no,at_beg_line\n");
}

void token_debug_info(Token * tok) {
  Token * tmp = contains_var(tok, "bt");
  if(tmp) {
    do {
      if(tmp->loc[0] != ',' && tmp->loc[0] != ';')
        for(int i = 0; i < tmp->len; i++)
          printf("%c", tmp->loc[i]);
      else if(tmp->loc[0] == ',')
        printf("comma");
      else if(tmp->loc[0] == ';')
        printf("semicolon");
      printf(",%s,", tokenkind_to_string(tmp->kind));
      if(tmp->kind == TK_NUM) {
        printf("%ld,", tmp->val);
        printf("%Lf,", tmp->fval);
      } else
        printf(",,");
      if(tmp->kind == TK_STR || tmp->kind == TK_NUM)
        type_debug_info(tmp->ty);
      else
        printf(",,,,,");
      // Length isn't Pertinent to AI!
      printf("%d,", tmp->line_no);
      printf("%s,", tmp->at_bol ? "at_bol" : "n_at_bol");
      printf("\n");
      tmp = tmp->next;
    } while(!tmp->at_bol);
    token_debug_info(tmp);
  }
  token_debug_info(tok->next);
}

Token * contains_var(Token * tok, const char * var) {
  Token * tmp = tok;
  do {
    if(!strncmp(tmp->loc, var, 2))
      return tok;
    tmp = tmp->next;
  } while(!tmp->at_bol);
  return  NULL;
}

void file_debug_info(File * file) {
  printf("%s, %d", file->name, file->file_no);
}

void type_debug_info(Type * type) {
  printf("%s,", typekind_to_string(type->kind));
  printf("%d,%d,%d,", type->size, type->align, no_nodes(type->vla_len, 0));
  // printf("%s", type->members ? "memstart," : ",");
  // printf("%s", type->origin ? typekind_to_string(type->kind) : ",");
  // printf("%s", type->origin ? "," : "");
  printf("%d,", type->array_len);
  // if(type->members) {
  //   member_type_debug_info(type->members);
  //   printf("memend,");
  // }
}

void member_type_debug_info(Member * member) {
  if(member) {
    printf("%s,", typekind_to_string(member->ty->kind));
    member_type_debug_info(member->next);
  }
}

int no_nodes(Node * n, int depth) {
  if(n)
    return no_nodes(n->next, depth + 1);
  else
    return depth;
}

const char * tokenkind_to_string(TokenKind tok_kind) {
  switch(tok_kind) {
    case TK_IDENT:   return "tk_ident";
    case TK_PUNCT:   return "tk_punct";
    case TK_KEYWORD: return "tk_keyword";
    case TK_STR:     return "tk_str";
    case TK_NUM:     return "tk_num";
    case TK_PP_NUM:  return "tk_pp_num";
    case TK_EOF:     return "tk_eof";
  }
  return "";
}

const char * typekind_to_string(TypeKind type_kind) {
  switch(type_kind) {
    case TY_VOID:    return "ty_void";
    case TY_BOOL:    return "ty_bool";
    case TY_CHAR:    return "ty_char";
    case TY_SHORT:   return "ty_short";
    case TY_INT:     return "ty_int";
    case TY_LONG:    return "ty_long";
    case TY_FLOAT:   return "ty_float";
    case TY_DOUBLE:  return "ty_double";
    case TY_LDOUBLE: return "ty_ldouble";
    case TY_ENUM:    return "ty_enum";
    case TY_PTR:     return "ty_ptr";
    case TY_FUNC:    return "ty_func";
    case TY_ARRAY:   return "ty_array";
    case TY_VLA:     return "ty_vla";
    case TY_STRUCT:  return "ty_struct";
    case TY_UNION:   return "ty_union";
  }
  return "";
}

void obj_debug(Obj * ob) {
  Obj * tmp = NULL;
  while(ob) {
    if(ob->params) {
      do {
        tmp = tmp ? ob->params : tmp->next;
        type_debug_info(tmp->ty);
      } while(tmp);
      tmp = NULL;
    }
    ob = ob->next;
  }
  printf("\n");
}
