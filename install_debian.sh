dpkg -i bazel-bin/dreal_4.18.05.2_amd64.deb
[[ "$?" != 0 ]] && { >&2 echo "Failed installation of debian package." ; exit 3 ; }
cp dreal/solver/sat_solver.h /opt/dreal/4.18.05.2/include/dreal/solver/
[[ "$?" != 0 ]] && { >&2 echo "Failed installation of extra files." ; exit 4 ; }
cp /data/projects/dreal4/bazel-dreal4/external/picosat/picosat.h /opt/dreal/4.18.05.2/include/dreal/solver/
[[ "$?" != 0 ]] && { >&2 echo "Failed installation of extra files." ; exit 4 ; }
echo "Installation completed."
