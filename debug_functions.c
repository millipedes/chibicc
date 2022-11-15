#include "chibicc.h"

void token_debug_info(Token * tok) {
  // kind,val,fval,*ty*,loc,*file*,line_no,beg
  Token * tmp = next_var_line(tok, "bt");
  if(tmp) {
    while(!tmp->at_bol) {
      printf("%s,", tokenkind_to_string(tmp->kind));
      if(tmp->kind == TK_NUM) {
        printf("%ld,", tmp->val);
        printf("%Lf,", tmp->fval);
      } else
        printf(",,");
      if(tmp->kind == TK_STR || tmp->kind == TK_NUM)
        type_debug_info(tmp->ty);
      printf("%s,", tmp->loc);
      // Length isn't Pertinent to AI!
      file_debug_info(tmp->file);
      printf("%d,", tmp->line_no);
      printf("%s,", tmp->at_bol ? "y" : "n");
      printf("\n");
      tmp = tmp->next;
    }
    token_debug_info(tmp);
  }
}

Token * next_var_line(Token * tok, const char * var) {
  Token * tmp;
  Token * line_start;
  int has_var = 0;
  while(!has_var && tok)
    for(tmp = tok, line_start = tmp; !(tmp->at_bol); tmp = tmp->next)
      if(!strncmp(tok->loc, var, sizeof(var)))
        has_var = 1;
  if(has_var)
    return line_start;
  else
    return NULL;
}

void file_debug_info(File * file) {
  printf("%s, %d", file->name, file->file_no);
}

void type_debug_info(Type * type) {
  printf("%s,", typekind_to_string(type->kind));
  printf("%d,%d,%d,", type->size, type->align, no_nodes(type->vla_len, 0));
  printf("%s", type->members ? "memstart," : ",");
  if(type->members) {
    member_type_debug_info(type->members);
    printf("memend,");
  }
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
