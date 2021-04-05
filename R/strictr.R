## usethis namespace: start
#' @useDynLib strictr, .registration = TRUE, .fixes = ""
## usethis namespace: end
NULL

.state <- new.env(parent = emptyenv())

#' @export
initialize_strictr <- function(logdir = getwd(), sigtype = "signature+force+effect+reflection") {

    logdir <- file.path(logdir, sigtype)

    if(!dir.exists(logdir)) {
        dir.create(logdir, recursive = TRUE, showWarnings = FALSE)
    }

    log_filepath <- file.path(logdir, "strictr-log")

    cache_dir <- system.file(file.path("signatures", sigtype), package = "strictr")

    if(cache_dir == "") {
        cache_dir <- "this-does-not-exist"
    }

    .Call(r_strictr_initialize_strictr, log_filepath, cache_dir)

    attach_callbacks()

    ## to not get anything printed by default on console
    invisible(NULL)
}

#' @export
finalize_strictr <- function(dir, sigtype, writer) {

    detach_callbacks()

    df <- .Call(r_strictr_finalize_strictr)

    filepath <- file.path(dir, sigtype, "application.fst")

    writer(df, filepath)

    invisible(df)
}

#' @importFrom utils installed.packages
attach_callbacks <- function() {
    handle_package_on_load <- function(libname, pkgname) {
        .Call(r_strictr_package_load_callback, libname, pkgname)
    }

    ## already loaded packages are handled internally on initialization
    .state$packages <- setdiff(unname(installed.packages()[,1]), loadedNamespaces())

    ## attach event hooks for packages not loaded
    for (package in .state$packages) {
        setHook(packageEvent(package, "onLoad"), handle_package_on_load, action = "prepend")
    }
}

detach_callbacks <- function() {
    ## attach event hooks for packages not loaded
    for (package in .state$packages) {
        setHook(packageEvent(package, "onLoad"), NULL, action = "replace")
    }

    .state$packages <- NULL
}
