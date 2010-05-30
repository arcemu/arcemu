AC_DEFUN([LIBGOMP_CHECK_SYNC_BUILTINS],
  [
    AC_CACHE_CHECK([whether the target supports __sync_*_compare_and_swap], libgomp_cv_have_sync_builtins,
      [
        AC_TRY_LINK([], [int foo, bar; bar = __sync_val_compare_and_swap(&foo, 0, 1);], libgomp_cv_have_sync_builtins=yes, libgomp_cv_have_sync_builtins=no)
      ]
    )
  ]
)

