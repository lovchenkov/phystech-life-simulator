FROM alpine

RUN apk add git

RUN apk add cmake
RUN apk add build-base

COPY . .
WORKDIR .
RUN mkdir build; \
    cd build; \
    cmake ..; \
    make ; \
    cd -

CMD ["build/main"]
