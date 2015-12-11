AR = ar
RANLIB = ranlib

DOIMAGE_FOLDER = marvell/doimage_mv
DOIMAGE_BINARY = doimage
POLARSSL_LIB_FOLDER = $(DOIMAGE_FOLDER)/polarssl/library
POLARSSL_LIB = libpolarssl.a

LIB_INCLUDE = $(top)/util/marvell/doimage_mv/polarssl/include
LIB_PATH = $(objutil)/$(DOIMAGE_FOLDER)/polarssl/library

CFLAGS  = -g -O1 -I./../inc -I$(LIB_INCLUDE) -DMV_CPU_LE
LDFLAGS = -L$(LIB_PATH) -lpolarssl

CFLAGS_POLARSSL = -g -I$(LIB_INCLUDE) -D_FILE_OFFSET_BITS=64

doimage_obj :=
doimage_obj += doimage.o

libpolarssl_obj :=
libpolarssl_obj += bignum.o        havege.o	rsa.o	sha2.o \
			sha1.o		timing.o

$(objutil)/$(DOIMAGE_FOLDER):
	mkdir -p $@

$(objutil)/$(POLARSSL_LIB_FOLDER):
	mkdir -p $@

$(objutil)/$(POLARSSL_LIB_FOLDER)/%.o: $(top)/util/marvell/doimage_mv/polarssl/library/%.c
	$(HOSTCC) $(CFLAGS_POLARSSL) -O -c $< -o $@

$(objutil)/$(POLARSSL_LIB_FOLDER)/$(POLARSSL_LIB): $(objutil)/$(POLARSSL_LIB_FOLDER) $(addprefix $(objutil)/$(POLARSSL_LIB_FOLDER)/,$(libpolarssl_obj))
	$(AR) r $@ $(addprefix $(objutil)/$(POLARSSL_LIB_FOLDER)/,$(libpolarssl_obj))
	$(RANLIB) $@

$(objutil)/$(DOIMAGE_FOLDER)/%.o: $(top)/util/marvell/doimage_mv/%.c
	$(HOSTCC) $(CFLAGS) -c $< -o $@

$(objutil)/$(DOIMAGE_FOLDER)/$(DOIMAGE_BINARY): $(objutil)/$(DOIMAGE_FOLDER)/doimage.o $(objutil)/$(POLARSSL_LIB_FOLDER)/$(POLARSSL_LIB)
	$(HOSTCC) $(CFLAGS) $(LDFLAGS) -o $@ $^
