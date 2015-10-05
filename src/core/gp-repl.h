#ifndef _GP_REPL_H
#define _GP_REPL_H

#include <glib-object.h>

G_BEGIN_DECLS

typedef enum
{
    GP_REPL_STATE_MAIN,
    GP_REPL_STATE_EVAL
} GPReplState;

#define GP_TYPE_REPL_STATE (gp_repl_state_get_type ())

GType gp_repl_state_get_type (void);

#define GP_TYPE_REPL (gp_repl_get_type ())

G_DECLARE_FINAL_TYPE (GPRepl, gp_repl, GP, REPL, GObject);

struct _GPRepl
{
    GObject parent_instance;
};

GPRepl *gp_repl_new (void);

G_END_DECLS

#endif /* _GP_REPL_H */
