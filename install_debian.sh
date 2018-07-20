rm -rf /opt/dreal

dpkg -i bazel-bin/dreal_4.18.05.2_amd64.deb
[[ "$?" != 0 ]] && { >&2 echo "Failed installation of debian package." ; exit 3 ; }

errors=0

mkdir -p /opt/dreal/4.18.05.2/include/dreal/util/fmt ; errors=$(($errors + $?))
mkdir -p /opt/dreal/4.18.05.2/include/dreal/util/spdlog/fmt ; errors=$(($errors + $?))
mkdir -p /opt/dreal/4.18.05.2/include/dreal/util/spdlog/fmt/bundled ; errors=$(($errors + $?))
mkdir -p /opt/dreal/4.18.05.2/include/dreal/util/spdlog/details ; errors=$(($errors + $?))

cp bazel-dreal4/external/fmt/fmt/ostream.h /opt/dreal/4.18.05.2/include/dreal/util/fmt ; errors=$(($errors + $?))
cp bazel-dreal4/external/fmt/fmt/format.h /opt/dreal/4.18.05.2/include/dreal/util/fmt ; errors=$(($errors + $?))
cp bazel-dreal4/external/spdlog/include/spdlog/fmt/fmt.h /opt/dreal/4.18.05.2/include/dreal/util/spdlog/fmt/ ; errors=$(($errors + $?))
cp bazel-dreal4/external/spdlog/include/spdlog/fmt/bundled/format.h /opt/dreal/4.18.05.2/include/dreal/util/spdlog/fmt/bundled/ ; errors=$(($errors + $?))
cp ./dreal/util/tseitin_cnfizer.h /opt/dreal/4.18.05.2/include/dreal/util/ ; errors=$(($errors + $?))
cp ./dreal/util/naive_cnfizer.h /opt/dreal/4.18.05.2/include/dreal/util/ ; errors=$(($errors + $?))
cp ./dreal/util/nnfizer.h /opt/dreal/4.18.05.2/include/dreal/util/ ; errors=$(($errors + $?))
cp ./dreal/util/predicate_abstractor.h /opt/dreal/4.18.05.2/include/dreal/util/ ; errors=$(($errors + $?))
cp bazel-dreal4/external/spdlog/include/spdlog/details/null_mutex.h /opt/dreal/4.18.05.2/include/dreal/util/spdlog/details ; errors=$(($errors + $?))
cp bazel-dreal4/external/spdlog/include/spdlog/spdlog.h /opt/dreal/4.18.05.2/include/dreal/util/spdlog ; errors=$(($errors + $?))
cp bazel-dreal4/external/spdlog/include/spdlog/tweakme.h /opt/dreal/4.18.05.2/include/dreal/util/spdlog ; errors=$(($errors + $?))
cp bazel-dreal4/external/spdlog/include/spdlog/common.h /opt/dreal/4.18.05.2/include/dreal/util/spdlog ; errors=$(($errors + $?))
cp xtra/dreal_pc.macos /opt/dreal/4.18.05.2/lib/pkgconfig/dreal.pc ; errors=$(($errors + $?))

if [[ $errors > 0 ]]
then { >&2 echo "There were $errors errors during installation of dreal files." ; exit 3 ; }
fi

cp dreal/solver/sat_solver.h /opt/dreal/4.18.05.2/include/dreal/solver/
[[ "$?" != 0 ]] && { >&2 echo "Failed installation of extra files." ; exit 4 ; }
cp /data/projects/dreal4/bazel-dreal4/external/picosat/picosat.h /opt/dreal/4.18.05.2/include/dreal/solver/
[[ "$?" != 0 ]] && { >&2 echo "Failed installation of extra files." ; exit 4 ; }
echo "Installation completed."
