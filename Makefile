export CC = avr-gcc
export LD = avr-gcc
export MCU = atmega32u4
export FCPU = 16000000
export FLAGS = -mmcu=$(MCU)
export CFLAGS = -Wall $(FLAGS) -DF_CPU=$(FCPU) -Os -I$(INCLUDEDIR)
export LDFLAGS = $(FLAGS)

export PROGRAMMER = dfu-programmer

TARGET = pad
SRCDIR = src
INCLUDEDIR = include
DISTDIR = dist

INCLUDES = $(wildcard $(INCLUDEDIR)/*.h)
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(DISTDIR)/%.o, $(SOURCES))

all: $(DISTDIR) $(DISTDIR)/$(TARGET).hex

$(DISTDIR):
	mkdir -p $(DISTDIR)

# Compilation des fichiers .c en .o
$(DISTDIR)/%.o: $(SRCDIR)/%.c | $(DISTDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Edition de liens
$(DISTDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# Génération du fichier .hex
$(DISTDIR)/$(TARGET).hex: $(DISTDIR)/$(TARGET)
	avr-objcopy -j .text -j .data -O ihex $< $@

# Nettoyage des fichiers compilés
clean:
	rm -rf $(DISTDIR)

# Upload du programme sur la carte
upload: $(DISTDIR)/$(TARGET).hex
	$(PROGRAMMER) $(MCU) erase
	$(PROGRAMMER) $(MCU) flash $<
	$(PROGRAMMER) $(MCU) reset

# Affichage de la taille du programme
size: $(DISTDIR)/$(TARGET)
	avr-size --format=avr --mcu=$(MCU) $<