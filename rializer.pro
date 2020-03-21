TEMPLATE = subdirs

SUBDIRS = sources \
          tests

tests.subdir = tests/auto
tests.depends = sources
