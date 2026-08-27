# Stage-1: build
FROM alpine:3.19 AS builder

# Install build tools
RUN  apk add --no-cache g++ make

# Set the working directory
WORKDIR /app

# Copy project contents 
COPY . .

RUN make


# Stage-2: Minimal Runtime
FROM alpine:3.19 AS runner

RUN apk add --no-cache libstdc++ libgcc

WORKDIR /app

# Copy ONLY the server binary output by your Makefile
COPY --from=builder /app/build/v_server .

# Create assets directory for creation and usage of 'cache_data.txt'
RUN mkdir -p ./assets

# Run the server binary on startup
CMD ["./v_server"]
