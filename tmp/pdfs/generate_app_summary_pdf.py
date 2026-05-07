from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


ROOT = Path("/Users/itsadoums/dev/unif/ba2/group-13")
OUTPUT_PATH = ROOT / "output/pdf/rpg_creator_app_summary.pdf"
FONT_PATH = ROOT / "res/police/ARIAL.TTF"

PAGE_WIDTH = 1800
PAGE_HEIGHT = 2550
MARGIN_X = 120
MARGIN_TOP = 110
MARGIN_BOTTOM = 110
CONTENT_WIDTH = PAGE_WIDTH - (MARGIN_X * 2)

BG = "#F5F1E8"
TEXT = "#17212B"
MUTED = "#51606F"
ACCENT = "#235789"
SECTION_BG = "#E5ECF3"
RULE = "#D3DBE4"


CONTENT = {
    "title": "RPG Creator",
    "subtitle": "One-page app summary from repo evidence",
    "what_it_is": (
        "A C++ desktop app for building RPG-style maps with a separate TCP server "
        "for shared projects. The repo shows pixel and sprite editing, project "
        "sharing, chat, and export/import flows."
    ),
    "who_its_for": (
        "Primary persona: RPG or game-map creators collaborating on shared 2D map projects."
    ),
    "what_it_does": [
        "Authenticates users and loads their project list.",
        "Creates, renames, duplicates, deletes, shares, and joins projects.",
        "Edits maps with pixel layers and sprite layers.",
        "Draws and erases square, circle, and diamond brush shapes.",
        "Places, moves, resizes, rotates, and removes sprites.",
        "Manages members and roles: spectator, editor, owner.",
        "Supports chat plus PNG and native .natif export/import.",
    ],
    "how_it_works": [
        "Client: SFML + TGUI desktop UI with Qt-based data handling. `Application` swaps `LoginView`, `MenuView`, and `GameView`, while `Project`, `Map`, `ToolBar`, and `Chat` hold editor state.",
        "Transport: `ClientNetworkManager` sends protocol messages over TCP to `127.0.0.1:5001`; `ServerNetworkManager` accepts sockets, queues requests, and sends responses.",
        "Server/data: `Worker` mutates in-memory `LiveProject` sessions and `LayerManager`; `SaveWorker` persists project JSON, images, zipped exports, and chat via `ProjectsManager`, while `DatabaseManager` stores users, projects, roles, and share tokens in SQLite `db.sqlite`.",
    ],
    "how_to_run": [
        "From repo root: `chmod +x install.sh` then `./install.sh`.",
        "The documented install path is apt-based Linux; `install.sh` uses `sudo apt` and builds into `bin/`.",
        "Start the server from `bin/`: `./server`.",
        "Start the client from `bin/` in another terminal: `./client`.",
    ],
}


def load_font(size: int) -> ImageFont.FreeTypeFont:
    return ImageFont.truetype(str(FONT_PATH), size=size)


def text_width(draw: ImageDraw.ImageDraw, text: str, font: ImageFont.ImageFont) -> int:
    left, _, right, _ = draw.textbbox((0, 0), text, font=font)
    return right - left


def wrap_text(draw: ImageDraw.ImageDraw, text: str, font: ImageFont.ImageFont, max_width: int):
    words = text.split()
    lines = []
    current = []
    for word in words:
        trial = " ".join(current + [word])
        if current and text_width(draw, trial, font) > max_width:
            lines.append(" ".join(current))
            current = [word]
        else:
            current.append(word)
    if current:
        lines.append(" ".join(current))
    return lines


def draw_paragraph(draw, x, y, text, font, fill, max_width, line_gap=10):
    lines = wrap_text(draw, text, font, max_width)
    bbox = draw.textbbox((0, 0), "Ag", font=font)
    line_height = bbox[3] - bbox[1]
    for line in lines:
        draw.text((x, y), line, font=font, fill=fill)
        y += line_height + line_gap
    return y


def draw_bullets(draw, x, y, items, font, fill, max_width, bullet_gap=10, item_gap=12):
    bullet_width = text_width(draw, "- ", font)
    for item in items:
        lines = wrap_text(draw, item, font, max_width - bullet_width)
        bbox = draw.textbbox((0, 0), "Ag", font=font)
        line_height = bbox[3] - bbox[1]
        for index, line in enumerate(lines):
            prefix = "- " if index == 0 else "  "
            draw.text((x, y), prefix + line, font=font, fill=fill)
            y += line_height + bullet_gap
        y += item_gap
    return y


def draw_section_header(draw, x, y, text, header_font):
    padding_x = 18
    padding_y = 12
    bbox = draw.textbbox((0, 0), text, font=header_font)
    width = bbox[2] - bbox[0]
    height = bbox[3] - bbox[1]
    draw.rounded_rectangle(
        (
            x,
            y,
            x + width + padding_x * 2,
            y + height + padding_y * 2,
        ),
        radius=18,
        fill=SECTION_BG,
    )
    draw.text((x + padding_x, y + padding_y - 2), text, font=header_font, fill=ACCENT)
    return y + height + padding_y * 2 + 18


def main():
    image = Image.new("RGB", (PAGE_WIDTH, PAGE_HEIGHT), BG)
    draw = ImageDraw.Draw(image)

    title_font = load_font(88)
    subtitle_font = load_font(36)
    header_font = load_font(42)
    body_font = load_font(40)
    bullet_font = load_font(38)

    y = MARGIN_TOP

    draw.rounded_rectangle((MARGIN_X, y, PAGE_WIDTH - MARGIN_X, y + 182), radius=30, fill=ACCENT)
    draw.text((MARGIN_X + 34, y + 14), CONTENT["title"], font=title_font, fill="white")
    draw.text((MARGIN_X + 36, y + 116), CONTENT["subtitle"], font=subtitle_font, fill="#DDEAF5")
    y += 220

    draw.line((MARGIN_X, y, PAGE_WIDTH - MARGIN_X, y), fill=RULE, width=3)
    y += 34

    y = draw_section_header(draw, MARGIN_X, y, "What it is", header_font)
    y = draw_paragraph(draw, MARGIN_X, y, CONTENT["what_it_is"], body_font, TEXT, CONTENT_WIDTH, line_gap=8)
    y += 18

    y = draw_section_header(draw, MARGIN_X, y, "Who it is for", header_font)
    y = draw_paragraph(draw, MARGIN_X, y, CONTENT["who_its_for"], body_font, TEXT, CONTENT_WIDTH, line_gap=8)
    y += 18

    y = draw_section_header(draw, MARGIN_X, y, "What it does", header_font)
    y = draw_bullets(draw, MARGIN_X, y, CONTENT["what_it_does"], bullet_font, TEXT, CONTENT_WIDTH, bullet_gap=6, item_gap=6)
    y += 8

    y = draw_section_header(draw, MARGIN_X, y, "How it works", header_font)
    y = draw_bullets(draw, MARGIN_X, y, CONTENT["how_it_works"], bullet_font, TEXT, CONTENT_WIDTH, bullet_gap=6, item_gap=6)
    y += 8

    y = draw_section_header(draw, MARGIN_X, y, "How to run", header_font)
    y = draw_bullets(draw, MARGIN_X, y, CONTENT["how_to_run"], bullet_font, TEXT, CONTENT_WIDTH, bullet_gap=6, item_gap=6)

    if y > PAGE_HEIGHT - MARGIN_BOTTOM:
        raise RuntimeError(f"Content overflowed the page: y={y}, limit={PAGE_HEIGHT - MARGIN_BOTTOM}")

    OUTPUT_PATH.parent.mkdir(parents=True, exist_ok=True)
    image.save(OUTPUT_PATH, "PDF", resolution=200.0)
    print(OUTPUT_PATH)


if __name__ == "__main__":
    main()
