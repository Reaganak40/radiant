import os

base_dir = os.path.abspath(os.pardir)

if (os.path.basename(base_dir) != 'Radiant'):
    err_msg = f"purge_visual_studio_files - BaseDir is not Radiant Project: {base_dir}"
    raise Exception(err_msg)

def purge_files(parent_dir):
    if not os.path.exists(parent_dir):
        err_msg = f"purge_visual_studio_files - Directory not found: {parent_dir}"
        raise Exception(err_msg)
        
    target_extensions = [".sln", ".vcxproj", ".vcxproj.filters"]
    
    for extension in target_extensions:
        for file in os.listdir(parent_dir):
            if file.endswith(extension):
                purge_file = os.path.join(parent_dir, file);
                print(f"Deleting: {purge_file}")
                os.remove(purge_file)
                
target_dirs = [
    "",
    "thirdparty/AL",
    "thirdparty/glad",
    "thirdparty/glfw",
    "thirdparty/imgui",
    "thirdparty/glm",
    
    "engine/src/Utils",
    "engine/src/Window",
    "engine/src/Graphics",
    "engine/src/Logger",
    "engine/src/Physics",
    "engine/src/Audio",
    "engine/src/ECS",
    "engine/src/Scene",
    "engine/src/Editor",
    "engine/src/Application",
    
    "engine/src/Core/src/OpenGL",
    
    "tests/glCore",
    "sandbox",
]

for target in target_dirs:
    target_path = os.path.join(base_dir, target)
    purge_files(target_path)
