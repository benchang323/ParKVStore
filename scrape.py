import os

def scrape_trips():
    # Get all the files in trips folder
    trips_folder = 'Assignment 5/'
    trips_files = [os.path.join(trips_folder, file) for file in os.listdir(trips_folder) if file.endswith(('.py', '.sh', '.cpp'))]

    # Scrape all the code from the files
    code = ''
    for file_path in trips_files:
        with open(file_path, 'r') as f:
            for line in f:
                if not line.strip().startswith('//'):  # Skip lines starting with "//"
                    code += line

    # Write the code to a file
    with open('code.txt', 'w') as f:
        f.write(code)
        
scrape_trips()
