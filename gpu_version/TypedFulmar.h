#include "Core.h"

// Printing fields to streams
template<typename Location>
std::ostream & operator<<(std::ostream & os, TypedField<Location> & field) {
    return core_write_field(os, field);
}

// Typed expression

template<typename Location, typename Expr>
struct TypedExpr {
    Location typedef location;
    Expr typedef expr_type;

    const Expr expr;

    TypedExpr(const Expr & exprArg) : expr(exprArg) {}
};

// Types

(define/meta single-value)
(define/meta s-vol)

(define/meta (join-location l1 l2)
  [(single-value single-value) single-value]
  [(single-value ,l) l]
  [(,l single-value) l]
  [(,l ,l) l]

(define/meta (check-equal t1 t2)
  [(,t ,t) t])

(define/meta (extract-location t)
  [double SingleValue]
  [int SingleValue]
  [_ (#:location t)])

(define/meta (wrap-return a)
  [_ (field-expr initial a)]
  [(typed-expr ,location ,expr-type) expr-type]
  [double const-expr]
  [int const-expr])

(define/meta (bin-expr-type op arg1 arg2)
  [_ (bin-expr initial op (wrap-return arg1) (wrap-return arg2))])

(define/meta (bin-expr-return op arg1 arg2)
   [_ (typed-expr (join-location (extract-location arg1)
                                 (extract-location arg2))
                  (bin-expr-type op arg1 arg2))])

// Wrap function

const ConstExpr wrap(double valueArg) {
    return ConstExpr(valueArg);
}

const ConstExpr wrap(int valueArg) {
    return ConstExpr(valueArg);
}

template<typename Location>
const FieldExpr<Initial, TypedField<Location> > wrap(const TypedField<Location> & arg) {
    return FieldExpr<Initial, TypedField<Location> >(arg);
}

template<typename Location, typename ExprType>
const ExprType & wrap(const TypedExpr<Location, ExprType> & arg) {
    return arg.expr;
}


// Nebo assignment

template<typename FieldType, typename TypedExpr>
void operator<<=(FieldType & field, const TypedExpr & rhs) {
    (typedef/meta result
      (check-equal
        (join-location
          (#:location field-type)
          (extract-location typed-expr))
        (#:location field-type))

    core_assign(field, wrap(rhs));
}

// Generic interface function

template<typename Op, typename Arg1, typename Arg2>
typename BinExprReturn<Op, Arg1, Arg2>::result binExpr(const Arg1 & arg1, const Arg2 & arg2) {
    (typedef/meta expr-type (bin-expr-type op arg1 arg2))
    (typedef/meta return-type (bin-expr-return op arg1 arg2))

    return ReturnType(ExprType(wrap(arg1), wrap(arg2)));
}

// Specific interface functions

template<typename Arg1, typename Arg2>
(meta (bin-expr-return sum-op arg1 arg2) operator+(const Arg1 & arg1, const Arg2 & arg2) {
    return binExpr<SumOp>(arg1, arg2);
}

template<typename Arg1, typename Arg2>
(meta (bin-expr-return mult-op arg1 arg2) operator*(const Arg1 & arg1, const Arg2 & arg2) {
    return binExpr<MultOp>(arg1, arg2);
}
