
#' @export
#' @importFrom instrumentr trace_code get_exec_stats
trace_expr <- function(code,
                       environment = parent.frame(),
                       quote = TRUE,
                       cache_dir = system.file("strictsig", package = "strictr")) {
    tracer <- .Call(C_strictr_tracer_create, cache_dir)

    if(quote) {
        code <- substitute(code)
    }

    result <- trace_code(tracer, code, environment = environment, quote = FALSE)

    invisible(result)
}

#' @export
trace_file <- function(file,
                       environment = parent.frame(),
                       cache_dir = system.file("strictsig", package = "strictr")) {
    code <- parse(file = file)

    code <- as.call(c(`{`, code))

    trace_expr(code, quote = FALSE)
}

