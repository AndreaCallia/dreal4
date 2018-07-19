mkdir -p /tmp/drealtemp
tar -C /tmp/drealtemp/ -xvf bazel-bin/archive.tar.gz
[[ "$?" != 0 ]] && { >&2 echo "Failed uncompressing the macos archive." ; exit 1 ; }
rm -rf /usr/local/opt/dreal
mv /tmp/drealtemp/opt/dreal /usr/local/opt/dreal
[[ "$?" != 0 ]] && { >&2 echo "Failed installation of the macos archive." ; exit 2 ; }
rm -rf /tmp/drealtemp

errors=0

mkdir -p /usr/local/opt/dreal/4.18.05.2/include/dreal/util/fmt ; errors=$(($errors + $?))
mkdir -p /usr/local/opt/dreal/4.18.05.2/include/dreal/util/spdlog/fmt ; errors=$(($errors + $?))
mkdir -p /usr/local/opt/dreal/4.18.05.2/include/dreal/util/spdlog/fmt/bundled ; errors=$(($errors + $?))
mkdir -p /usr/local/opt/dreal/4.18.05.2/include/dreal/util/spdlog/details ; errors=$(($errors + $?))

cp bazel-dreal4/external/fmt/fmt/ostream.h /usr/local/opt/dreal/4.18.05.2/include/dreal/util/fmt ; errors=$(($errors + $?))
cp bazel-dreal4/external/fmt/fmt/format.h /usr/local/opt/dreal/4.18.05.2/include/dreal/util/fmt ; errors=$(($errors + $?))
cp bazel-dreal4/external/spdlog/include/spdlog/fmt/fmt.h /usr/local/opt/dreal/4.18.05.2/include/dreal/util/spdlog/fmt/ ; errors=$(($errors + $?))
cp bazel-dreal4/external/spdlog/include/spdlog/fmt/bundled/format.h /usr/local/opt/dreal/4.18.05.2/include/dreal/util/spdlog/fmt/bundled/ ; errors=$(($errors + $?))
cp ./dreal/util/tseitin_cnfizer.h /usr/local/opt/dreal/4.18.05.2/include/dreal/util/ ; errors=$(($errors + $?))
cp ./dreal/util/naive_cnfizer.h /usr/local/opt/dreal/4.18.05.2/include/dreal/util/ ; errors=$(($errors + $?))
cp ./dreal/util/nnfizer.h /usr/local/opt/dreal/4.18.05.2/include/dreal/util/ ; errors=$(($errors + $?))
cp ./dreal/util/predicate_abstractor.h /usr/local/opt/dreal/4.18.05.2/include/dreal/util/ ; errors=$(($errors + $?))
cp bazel-dreal4/external/spdlog/include/spdlog/details/null_mutex.h /usr/local/opt/dreal/4.18.05.2/include/dreal/util/spdlog/details ; errors=$(($errors + $?))
cp bazel-dreal4/external/spdlog/include/spdlog/spdlog.h /usr/local/opt/dreal/4.18.05.2/include/dreal/util/spdlog ; errors=$(($errors + $?))
cp bazel-dreal4/external/spdlog/include/spdlog/tweakme.h /usr/local/opt/dreal/4.18.05.2/include/dreal/util/spdlog ; errors=$(($errors + $?))
cp bazel-dreal4/external/spdlog/include/spdlog/common.h /usr/local/opt/dreal/4.18.05.2/include/dreal/util/spdlog ; errors=$(($errors + $?))
cp xtra/dreal_pc.macos /usr/local/opt/dreal/4.18.05.2/lib/pkgconfig/dreal.pc ; errors=$(($errors + $?))

if [[ $errors > 0 ]]
then { >&2 echo "There were $errors errors during installation of dreal files." ; exit 3 ; }
fi

cp dreal/solver/sat_solver.h /usr/local/opt/dreal/4.18.05.2/include/dreal/solver/
[[ "$?" != 0 ]] && { >&2 echo "Failed installation of extra files." ; exit 4 ; }
cp smts/picosat.h /usr/local/opt/dreal/4.18.05.2/include/dreal/solver/
[[ "$?" != 0 ]] && { >&2 echo "Failed installation of extra files." ; exit 4 ; }
echo "Installation completed."
