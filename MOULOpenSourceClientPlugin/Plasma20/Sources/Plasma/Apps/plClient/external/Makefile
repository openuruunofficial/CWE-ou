SOURCE_DIR = .
BINARY_DIR = .

.PHONY: all clean

all: $(BINARY_DIR)/resource.dat

$(BINARY_DIR)/resource.dat: $(SOURCE_DIR)/makeres.py $(SOURCE_DIR)/render_svg.py $(SOURCE_DIR)/create_resource_dat.py $(SOURCE_DIR)/Cursor_Base.svg $(SOURCE_DIR)/Linking_Book.svg $(SOURCE_DIR)/Loading_Text_rasterfont.svg $(SOURCE_DIR)/Voice_Chat.svg
	python $(SOURCE_DIR)/makeres.py --optimize --render --package -i $(SOURCE_DIR) -o $(BINARY_DIR)

clean:
	rm -rf $(BINARY_DIR)/render
	rm -fv $(BINARY_DIR)/resource.dat
