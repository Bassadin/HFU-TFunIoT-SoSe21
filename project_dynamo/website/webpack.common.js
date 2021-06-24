const path = require("path");
const HtmlWebpackPlugin = require("html-webpack-plugin");

module.exports = {
    entry: {
        main: "./src/index.ts"
    },
    plugins: [
        new HtmlWebpackPlugin({
            template: "./src/index.html", // Damit gibt man an, dass die HTML dynamisch erzeugt werden soll mit eben jenem Template. Das brauchen wir weil das JS-File dynamisch reinkommen soll
            inject: "body",
            scriptLoading: "blocking",
        }),
    ],
    module: {
        rules: [
            {
                test: /\.html$/,
                loader: "html-loader",
                options: {
                    esModule: false,
                },
            },
            {
                test: /\.(svg|png|jpe?g|gif)$/,
                use: {
                    loader: "file-loader",
                    options: {
                        name: "[hash:6].[ext]",
                        outputPath: "img",
                    },
                },
            },
            {
                test: /\.(woff|woff2|eot|ttf|otf)$/i,
                type: 'asset/resource',
            },
            {
                test: /\.tsx?$/,
                use: 'ts-loader',
                exclude: /node_modules/,
            },
        ],
    },
    resolve: {
        extensions: ['.tsx', '.ts', '.js'],
    },
};
