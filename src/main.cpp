// TrafficVisionAI - Checkpoint 0
//
// Aplicacion de consola minima que valida la integracion de OpenCV.
// Soporta tres modos de entrada: imagen, video y camara.
//
// Nota: los mensajes se escriben en ASCII para que se muestren correctamente
// en la consola de Windows independientemente de la pagina de codigos activa.

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

#include <filesystem>
#include <iostream>
#include <string>

namespace {

// Codigo de la tecla ESC devuelto por cv::waitKey.
constexpr int kEscapeKey = 27;

// Retardo por defecto entre frames cuando la fuente no reporta unos FPS validos.
constexpr int kDefaultFrameDelayMs = 30;

void printUsage() {
    std::cout << "TrafficVisionAI - Checkpoint 0\n"
              << "\n"
              << "Uso:\n"
              << "  TrafficVisionAI image <ruta_imagen>\n"
              << "  TrafficVisionAI video <ruta_video>\n"
              << "  TrafficVisionAI camera [indice]\n"
              << "\n"
              << "En los modos video y camara, pulsa ESC para salir.\n";
}

// Comprueba que la ruta indicada apunta a un archivo existente.
bool isExistingFile(const std::string& path) {
    std::error_code ec;
    return std::filesystem::is_regular_file(path, ec);
}

int runImageMode(const std::string& imagePath) {
    if (!isExistingFile(imagePath)) {
        std::cerr << "Error: la ruta no existe o no es un archivo: " << imagePath << "\n";
        return 1;
    }

    // IMREAD_UNCHANGED conserva el numero real de canales del archivo.
    const cv::Mat image = cv::imread(imagePath, cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cerr << "Error: OpenCV no pudo leer la imagen: " << imagePath << "\n";
        return 1;
    }

    std::cout << "Imagen  : " << imagePath << "\n"
              << "  Ancho   : " << image.cols << " px\n"
              << "  Alto    : " << image.rows << " px\n"
              << "  Canales : " << image.channels() << "\n"
              << "\nPulsa cualquier tecla sobre la ventana para cerrar." << std::endl;

    const std::string windowName = "TrafficVisionAI - imagen";
    cv::imshow(windowName, image);
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}

// Muestra los frames de una captura ya abierta hasta que se agote la fuente
// o el usuario pulse ESC. Devuelve 0 si la reproduccion termino correctamente.
int displayCaptureLoop(cv::VideoCapture& capture,
                       const std::string& windowName,
                       int frameDelayMs) {
    cv::Mat frame;
    while (true) {
        if (!capture.read(frame) || frame.empty()) {
            break;  // Fin de la fuente.
        }

        cv::imshow(windowName, frame);
        if (cv::waitKey(frameDelayMs) == kEscapeKey) {
            std::cout << "Reproduccion interrumpida por el usuario (ESC).\n";
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}

int runVideoMode(const std::string& videoPath) {
    if (!isExistingFile(videoPath)) {
        std::cerr << "Error: la ruta no existe o no es un archivo: " << videoPath << "\n";
        return 1;
    }

    cv::VideoCapture capture(videoPath);
    if (!capture.isOpened()) {
        std::cerr << "Error: OpenCV no pudo abrir el video: " << videoPath << "\n";
        return 1;
    }

    const double fps = capture.get(cv::CAP_PROP_FPS);
    const int width = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    const int height = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));

    std::cout << "Video   : " << videoPath << "\n"
              << "  FPS        : " << fps << "\n"
              << "  Resolucion : " << width << " x " << height << "\n"
              << "\nPulsa ESC para salir." << std::endl;

    // Retardo aproximado a partir de los FPS declarados por la fuente.
    // No es una sincronizacion precisa; solo evita reproducir a maxima velocidad.
    const int frameDelayMs =
        (fps > 0.0) ? static_cast<int>(1000.0 / fps) : kDefaultFrameDelayMs;

    return displayCaptureLoop(capture, "TrafficVisionAI - video",
                              (frameDelayMs > 0) ? frameDelayMs : 1);
}

int runCameraMode(int cameraIndex) {
    cv::VideoCapture capture(cameraIndex);
    if (!capture.isOpened()) {
        std::cerr << "Error: no se pudo abrir la camara con indice " << cameraIndex << ".\n";
        return 1;
    }

    const int width = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_WIDTH));
    const int height = static_cast<int>(capture.get(cv::CAP_PROP_FRAME_HEIGHT));

    std::cout << "Camara  : indice " << cameraIndex << "\n"
              << "  Resolucion : " << width << " x " << height << "\n"
              << "\nPulsa ESC para salir." << std::endl;

    return displayCaptureLoop(capture, "TrafficVisionAI - camara", 1);
}

// Convierte el argumento de indice de camara. Devuelve false si no es valido.
bool parseCameraIndex(const std::string& text, int& cameraIndex) {
    try {
        size_t consumed = 0;
        const int value = std::stoi(text, &consumed);
        if (consumed != text.size() || value < 0) {
            return false;
        }
        cameraIndex = value;
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    const std::string mode = argv[1];

    if (mode == "image") {
        if (argc < 3) {
            std::cerr << "Error: el modo 'image' requiere una ruta de imagen.\n\n";
            printUsage();
            return 1;
        }
        return runImageMode(argv[2]);
    }

    if (mode == "video") {
        if (argc < 3) {
            std::cerr << "Error: el modo 'video' requiere una ruta de video.\n\n";
            printUsage();
            return 1;
        }
        return runVideoMode(argv[2]);
    }

    if (mode == "camera") {
        int cameraIndex = 0;  // Indice por defecto.
        if (argc >= 3 && !parseCameraIndex(argv[2], cameraIndex)) {
            std::cerr << "Error: indice de camara no valido: " << argv[2] << "\n\n";
            printUsage();
            return 1;
        }
        return runCameraMode(cameraIndex);
    }

    std::cerr << "Error: modo desconocido: " << mode << "\n\n";
    printUsage();
    return 1;
}
