## usethis namespace: start
#' @useDynLib strictr, .registration = TRUE, .fixes = ""
## usethis namespace: end
NULL

#' @export
initialize_strictr <- function(log_filepath = file.path(getwd(), "siglog.txt"),
                               cache_dir = system.file("strictsig", package = "strictr")) {
    .Call(r_strictr_initialize_strictr, log_filepath, cache_dir)
    attach_callbacks()
    ## to not get anything printed by default on console
    invisible(NULL)
}

#' @export
finalize_strictr <- function() {
    .Call(r_strictr_finalize_strictr)
}

#' @importFrom utils installed.packages
attach_callbacks <- function() {
    handle_package_on_load <- function(libname, pkgname) {
        .Call(r_strictr_package_load_callback, libname, pkgname)
    }

    ## already loaded packages are handled internally on initialization
    future_packages <- setdiff(unname(installed.packages()[,1]), loadedNamespaces())

    ## attach event hooks for packages not loaded
    for (package in future_packages) {
        setHook(packageEvent(package, "onLoad"), handle_package_on_load, action = "prepend")
    }
}
