TEMPLATE = subdirs
SUBDIRS += \
    src    \
    app    \
    tests

app.depends = src
tests.depends = src
