#include "chibicc.h"

void token_debug_info(Token * tok) {
  // kind,val,fval,*ty*,loc,*file*,line_no,beg
  if(tok) {
    printf("%s,", tokenkind_to_string(tok->kind));
    if(tok->kind == TK_NUM) {
      printf("%ld,", tok->val);
      printf("%Lf,", tok->fval);
    } else
      printf(",,");
    if(tok->kind == TK_STR || tok->kind == TK_NUM)
      type_debug_info(tok->ty);
    printf("%s,", tok->loc);
    // Length isn't Pertinent to AI!
    file_debug_info(tok->file);
    printf("%d,", tok->line_no);
    printf("%s,", tok->at_bol ? "y" : "n");
    printf("\n");
    token_debug_info(tok->next);
  }
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
