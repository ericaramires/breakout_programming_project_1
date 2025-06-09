# ===== BREAKOUT GAME MAKEFILE =====
# Professional build system with automatic dependency tracking

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = 
LIBS = -lallegro -lallegro_main -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

# Directories
SRCDIR = .
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = main.c game.c player.c ball.c blocks.c particles.c io.c ui.c
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
DEPENDS = $(OBJECTS:%.o=%.d)

# Target executable
TARGET = $(BINDIR)/breakout_enhanced

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJDIR) $(BINDIR)

# Link the executable
$(TARGET): $(OBJECTS)
	@echo "Linking $@..."
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS) $(LIBS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Compile source files to object files with dependency generation
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEPENDS)

# Development build (with debug symbols and no optimization)
debug: CFLAGS = -Wall -Wextra -std=c99 -g -DDEBUG
debug: clean all

# Release build (optimized)
release: CFLAGS = -Wall -Wextra -std=c99 -O3 -DNDEBUG
release: clean all

# Install (copy to system directory)
install: $(TARGET)
	@echo "Installing breakout to /usr/local/bin..."
	@sudo cp $(TARGET) /usr/local/bin/breakout
	@echo "Installation complete!"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(OBJDIR) $(BINDIR)
	@echo "Clean complete!"

# Clean and rebuild
rebuild: clean all

# Run the game
run: $(TARGET)
	@./$(TARGET)

# Check for memory leaks (requires valgrind)
memcheck: $(TARGET)
	@echo "Running memory check..."
	@valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Show help
help:
	@echo "Breakout Game Build System"
	@echo "=========================="
	@echo "Available targets:"
	@echo "  all       - Build the game (default)"
	@echo "  debug     - Build with debug symbols"
	@echo "  release   - Build optimized version"
	@echo "  clean     - Remove build artifacts"
	@echo "  rebuild   - Clean and build"
	@echo "  run       - Build and run the game"
	@echo "  install   - Install to system"
	@echo "  memcheck  - Run with valgrind"
	@echo "  help      - Show this help"

# Package for distribution
package: release
	@echo "Creating distribution package..."
	@mkdir -p dist/breakout
	@cp $(TARGET) dist/breakout/
	@cp README.md dist/breakout/
	@cp -r assets dist/breakout/ 2>/dev/null || true
	@cd dist && tar -czf breakout-game.tar.gz breakout/
	@echo "Package created: dist/breakout-game.tar.gz"

# Phony targets (not actual files)
.PHONY: all directories debug release install clean rebuild run memcheck help package 