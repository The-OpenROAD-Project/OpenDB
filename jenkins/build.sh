docker build --target base-dependencies -t opendb .
docker run -v $(pwd):/OpenDB opendb bash -c "./OpenDB/jenkins/install.sh"