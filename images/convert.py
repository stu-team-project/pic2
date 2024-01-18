from PIL import Image

def convert_to_ascii_ppm(image_path, output_path):
    with Image.open(image_path) as img:
        width, height = img.size
        pixels = img.load()

    ppm_header = f"P3\n{width} {height}\n255\n"
    ppm_body = ""

    for y in range(height):
        for x in range(width):
            r, g, b = pixels[x, y]
            ppm_body += f"{r} {g} {b} "

        ppm_body += "\n"

    with open(output_path, "w") as f:
        f.write(ppm_header + ppm_body)
